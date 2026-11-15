#include "otsdaq-components/SlowControlsInterfacePlugins/OtsSlowControlsInterface.h"
#include "otsdaq/Macros/SlowControlsPluginMacros.h"

#include <dirent.h>
#include <fstream>
#include <sstream>

using namespace ots;

OtsSlowControlsInterface::OtsSlowControlsInterface(
    const std::string&       pluginType,
    const std::string&       interfaceUID,
    const ConfigurationTree& theXDAQContextConfigTree,
    const std::string&       controlsConfigurationPath)
    : SlowControlsVInterface(
          pluginType, interfaceUID, theXDAQContextConfigTree, controlsConfigurationPath)
{
	// Values come from the aggregate text files that front-end interfaces write when
	// 'SlowControlsLocalAggregateSavingEnabled' is on (and 'SlowControlsSaveBinaryFile'
	// is off). Each sample is three lines: time (s), channel name, value.
	// Directory: $OTS_SLOW_CONTROLS_DATA_PATH, else $USER_DATA/ServiceData/SlowControlsData
	const char* envPath = getenv("OTS_SLOW_CONTROLS_DATA_PATH");
	if(envPath && envPath[0])
		dataPath_ = envPath;
	else
	{
		const char* userData = getenv("USER_DATA");
		dataPath_            = std::string(userData ? userData : ".") +
		            "/ServiceData/SlowControlsData";
	}
	__COUT_INFO__ << "OtsSlowControlsInterface reading aggregate files from: "
	              << dataPath_ << __E__;
}

OtsSlowControlsInterface::~OtsSlowControlsInterface() { destroy(); }

void OtsSlowControlsInterface::initialize() {}

void OtsSlowControlsInterface::destroy() {}

//==============================================================================
// scanFiles: caller must hold mutex_
void OtsSlowControlsInterface::scanFiles()
{
	time_t now = time(0);
	if(now == lastScan_)
		return;  // at most once per second
	lastScan_ = now;

	DIR* dir = opendir(dataPath_.c_str());
	if(!dir)
		return;
	std::vector<std::string> names;
	while(struct dirent* e = readdir(dir))
	{
		std::string n = e->d_name;
		if(n.size() > 4 && n.substr(n.size() - 4) == ".txt")
			names.push_back(n);
	}
	closedir(dir);

	for(const auto& n : names)
	{
		std::string   fullName = dataPath_ + "/" + n;
		std::ifstream in(fullName);
		if(!in)
			continue;
		std::streamoff& offset = fileOffsets_[fullName];
		in.seekg(offset);
		std::string t, name, value;
		while(true)
		{
			std::streamoff start = in.tellg();
			if(!std::getline(in, t) || !std::getline(in, name) ||
			   !std::getline(in, value) || in.eof())
			{
				// incomplete sample: try again at the next scan
				offset = start;
				break;
			}
			offset = in.tellg();
			auto& h = history_[name];
			h.push_back({t, value});
			if(h.size() > MAX_HISTORY_)
				h.pop_front();
		}
	}
}  // end scanFiles()

std::vector<std::string> OtsSlowControlsInterface::getChannelList()
{
	std::lock_guard<std::mutex> lock(mutex_);
	scanFiles();
	std::vector<std::string> names;
	for(const auto& h : history_)
		names.push_back(h.first);
	return names;
}

std::string OtsSlowControlsInterface::getList(const std::string& format)
{
	std::vector<std::string> names = getChannelList();
	if(format != "JSON")
	{
		std::string s;
		for(const auto& n : names)
			s += n + "\n";
		return s;
	}
	std::string json = "[";
	for(size_t i = 0; i < names.size(); ++i)
		json += (i ? ",\"" : "\"") + names[i] + "\"";
	return json + "]";
}
void OtsSlowControlsInterface::subscribe(const std::string& /*Name*/) {}

void OtsSlowControlsInterface::subscribeJSON(const std::string& /*List*/) {}

void OtsSlowControlsInterface::unsubscribe(const std::string& /*Name*/) {}

std::array<std::string, 4> OtsSlowControlsInterface::getCurrentValue(
    const std::string& Name)
{
	// {time, value, status, severity}; "NO_CHANGE" when nothing new since last call
	std::lock_guard<std::mutex> lock(mutex_);
	scanFiles();
	auto it = history_.find(Name);
	if(it == history_.end() || it->second.empty())
		return {"0", "", "NO_CONNECTION", "INVALID"};
	const Sample& s = it->second.back();
	return {s.time_, s.value_, "OK", "NO_ALARM"};
}

std::vector<std::vector<std::string>> OtsSlowControlsInterface::getChannelHistory(
    const std::string& Name, int startTime, int endTime)
{
	std::lock_guard<std::mutex> lock(mutex_);
	scanFiles();
	std::vector<std::vector<std::string>> out;
	auto                                  it = history_.find(Name);
	if(it == history_.end())
		return out;
	for(const auto& s : it->second)
	{
		long t = atol(s.time_.c_str());
		if(t >= startTime && (endTime <= 0 || t <= endTime))
			out.push_back({s.time_, s.value_, "OK", "NO_ALARM"});
	}
	return out;
}

std::vector<std::vector<std::string>> OtsSlowControlsInterface::getLastAlarms(
    const std::string& /*Name*/)
{
	return {};
}

std::vector<std::vector<std::string>> OtsSlowControlsInterface::getAlarmsLog(
    const std::string& /*Name*/)
{
	return {};
}

std::array<std::string, 9> OtsSlowControlsInterface::getSettings(
    const std::string& /*Name*/)
{
	// units, display hi/lo, alarm hi, warn hi, warn lo, alarm lo, control hi/lo
	return {"", "65535", "0", "", "", "", "", "", ""};
}

std::vector<std::vector<std::string>> OtsSlowControlsInterface::checkAlarmNotifications(
    void)
{
	return {};
}

DEFINE_OTS_SLOW_CONTROLS(OtsSlowControlsInterface)

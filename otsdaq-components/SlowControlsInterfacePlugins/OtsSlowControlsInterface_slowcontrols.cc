#include "otsdaq-components/SlowControlsInterfacePlugins/OtsSlowControlsInterface.h"
#include "otsdaq/Macros/SlowControlsPluginMacros.h"

using namespace ots;

OtsSlowControlsInterface::OtsSlowControlsInterface(
    const std::string&       pluginType,
    const std::string&       interfaceUID,
    const ConfigurationTree& theXDAQContextConfigTree,
    const std::string&       controlsConfigurationPath)
    // TODO -- listen on a socket for data
    //	or read from disk
    //  :Socket            (
    //  theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("HostIPAddress").getValue<std::string>()
    // ,theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("HostPort").getValue<unsigned
    // int>())
    // ,
    : SlowControlsVInterface(
          pluginType, interfaceUID, theXDAQContextConfigTree, controlsConfigurationPath)
{
}

OtsSlowControlsInterface::~OtsSlowControlsInterface() { destroy(); }

void OtsSlowControlsInterface::initialize() {}

void OtsSlowControlsInterface::destroy() {}

std::vector<std::string> OtsSlowControlsInterface::getChannelList() { return {}; }

std::string OtsSlowControlsInterface::getList(const std::string& /*format*/)
{
	//__COUT__ <<
	// theXDAQContextConfigTree.getNode(controlsConfigurationPath).getValue <<
	// std::endl;
	return (std::string) "list";
}
void OtsSlowControlsInterface::subscribe(const std::string& /*Name*/) {}

void OtsSlowControlsInterface::subscribeJSON(const std::string& /*List*/) {}

void OtsSlowControlsInterface::unsubscribe(const std::string& /*Name*/) {}

std::array<std::string, 4> OtsSlowControlsInterface::getCurrentValue(
    const std::string& /*Name*/)
{
	return {"a", "b", "c", "d"};
}

std::vector<std::vector<std::string>> OtsSlowControlsInterface::getChannelHistory(
    const std::string& /*Name*/
    ,
    int /*start time*/
    ,
    int /* end time*/)
{
	return {};
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
	return {"a", "b", "c", "d", "e", "f", "g", "h", "i"};
}

std::vector<std::vector<std::string>> OtsSlowControlsInterface::checkAlarmNotifications(
    void)
{
	return {};
}

DEFINE_OTS_SLOW_CONTROLS(OtsSlowControlsInterface)

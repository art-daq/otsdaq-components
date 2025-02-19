#ifndef _ots_OtsSlowControlsInterface_h
#define _ots_OtsSlowControlsInterface_h

#include <array>
#include <string>

#include "otsdaq/NetworkUtilities/ReceiverSocket.h"  // Make sure this is always first because <sys/types.h> (defined in Socket.h) must be first
#include "otsdaq/SlowControlsCore/SlowControlsVInterface.h"
using namespace ots;
//{

class OtsSlowControlsInterface : public SlowControlsVInterface
{
  public:
	OtsSlowControlsInterface(const std::string&       pluginType,
	                         const std::string&       interfaceUID,
	                         const ConfigurationTree& theXDAQContextConfigTree,
	                         const std::string&       controlsConfigurationPath);
	~OtsSlowControlsInterface();

	void initialize();
	void destroy();

	std::vector<std::string /*Name*/> getChannelList();
	std::string                       getList(const std::string& format);
	void                              subscribe(const std::string& Name);
	void                              subscribeJSON(const std::string& JSONNameString);
	void                              unsubscribe(const std::string& Name);
	std::array<std::string, 4>        getCurrentValue(const std::string& Name);
	std::vector<std::vector<std::string>> getChannelHistory(const std::string& Name,
	                                                        int                startTime,
	                                                        int                endTime);
	std::vector<std::vector<std::string>> getLastAlarms(const std::string& Name);
	std::vector<std::vector<std::string>> getAlarmsLog(const std::string& Name);
	std::vector<std::vector<std::string>> checkAlarmNotifications(void);
	std::array<std::string, 9>            getSettings(const std::string& Name);
};

///}
///
#endif

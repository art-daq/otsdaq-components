#ifndef _ots_FEOtsUDPFSSRInterfaceTable_h_
#define _ots_FEOtsUDPFSSRInterfaceTable_h_

#include <string>
#include "otsdaq/TableDataFormats/FEInterfaceTableBase.h"

namespace ots
{
class FEOtsUDPFSSRInterfaceTable : public FEInterfaceTableBase
{
  public:
	FEOtsUDPFSSRInterfaceTable(void);
	virtual ~FEOtsUDPFSSRInterfaceTable(void);

	/// Methods
	void init(ConfigurationManager* configManager);

	/// Getters
	bool         getStatus(std::string interfaceID) const;
	bool         getChannelStatus(std::string interfaceID, unsigned int channel) const;
	unsigned int getFirmwareVersion(std::string interfaceID) const;
	virtual std::string  getIPAddress(std::string interfaceID) const;
	virtual unsigned int getPort(std::string interfaceID) const;
	std::string          getInterfaceIPAddress(std::string interfaceID) const;
	unsigned int         getInterfacePort(std::string interfaceID) const;
	std::string          getClockSelect(std::string interfaceID) const;
	double               getClockSpeedMHz(std::string interfaceID) const;
	std::string          getStreamingIPAddress(std::string interfaceID) const;
	unsigned int         getStreamingPort(std::string interfaceID) const;

  private:
	enum
	{
		InterfaceID,
		Status,
		FirmwareVersion,
		IPAddress,
		Port,
		InterfaceIPAddress,
		InterfacePort,
		StreamingIPAddress,
		StreamingPort,
		ClockSelect,
		ClockSpeedMHz,
		ChannelStatus0,
		ChannelStatus1,
		ChannelStatus2,
		ChannelStatus3,
		ChannelStatus4,
		ChannelStatus5
	};
};
}  // namespace ots
#endif

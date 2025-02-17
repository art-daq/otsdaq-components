#ifndef _ots_FEWROtsUDPTemplateInterfaceTable_h_
#define _ots_FEWROtsUDPTemplateInterfaceTable_h_

#include <string>
#include "otsdaq/TableDataFormats/FEInterfaceTableBase.h"

namespace ots
{
class FEWROtsUDPTemplateInterfaceTable : public FEInterfaceTableBase
{
  public:
	FEWROtsUDPTemplateInterfaceTable(void);
	virtual ~FEWROtsUDPTemplateInterfaceTable(void);

	/// Methods
	void init(ConfigurationManager* configManager);

	/// Getters
	bool         getStatus(std::string interfaceID) const;
	bool         getChannelStatus(std::string interfaceID, unsigned int channel) const;
	unsigned int getFirmwareVersion(std::string interfaceID) const;
	std::string  getIPAddress(std::string interfaceID) const;
	unsigned int getPort(std::string interfaceID) const;
	std::string  getInterfaceIPAddress(std::string interfaceID) const;
	unsigned int getInterfacePort(std::string interfaceID) const;
	std::string  getStreamingIPAddress(std::string interfaceID) const;
	unsigned int getStreamingPort(std::string interfaceID) const;
	int          getNumberOfWords(std::string interfaceID) const;
	unsigned int getDataRate(std::string interfaceID) const;

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
		NumberOfWords,
		DataRate,
	};
};
}  // namespace ots
#endif

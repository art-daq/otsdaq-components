#ifndef OTSDAQ_COMPONENTS_OTSDAQ_COMPONENTS_USERTABLEDATAFORMATS_FEOTSUDPLCLS2INTERFACETABLE_H_
#define OTSDAQ_COMPONENTS_OTSDAQ_COMPONENTS_USERTABLEDATAFORMATS_FEOTSUDPLCLS2INTERFACETABLE_H_

#include <string>
#include "otsdaq/TableDataFormats/FEInterfaceTableBase.h"

namespace ots
{
class FEOtsUDPLCLS2InterfaceTable : public FEInterfaceTableBase
{
  public:
	FEOtsUDPLCLS2InterfaceTable(void);
	virtual ~FEOtsUDPLCLS2InterfaceTable(void);

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
#endif  // OTSDAQ_COMPONENTS_OTSDAQ_COMPONENTS_USERTABLEDATAFORMATS_FEOTSUDPLCLS2INTERFACETABLE_H_

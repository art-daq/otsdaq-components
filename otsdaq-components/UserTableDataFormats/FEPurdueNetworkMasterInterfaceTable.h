#ifndef OTSDAQ_COMPONENTS_OTSDAQ_COMPONENTS_USERTABLEDATAFORMATS_FEPURDUENETWORKMASTERINTERFACETABLE_H_
#define OTSDAQ_COMPONENTS_OTSDAQ_COMPONENTS_USERTABLEDATAFORMATS_FEPURDUENETWORKMASTERINTERFACETABLE_H_

#include <string>
#include "otsdaq/TableDataFormats/FEInterfaceTableBase.h"

namespace ots
{
class FEPurdueNetworkMasterInterfaceTable : public FEInterfaceTableBase
{
  public:
	FEPurdueNetworkMasterInterfaceTable(void);
	virtual ~FEPurdueNetworkMasterInterfaceTable(void);

	/// Methods
	void init(ConfigurationManager* configManager);

	/// Getters
	bool         getStatus(std::string interfaceID) const;
	unsigned int getFirmwareVersion(std::string interfaceID) const;
	std::string  getIPAddress(std::string interfaceID) const;
	unsigned int getPort(std::string interfaceID) const;
	std::string  getInterfaceIPAddress(std::string interfaceID) const;
	unsigned int getInterfacePort(std::string interfaceID) const;

  private:
	enum
	{
		InterfaceID,
		Status,
		FirmwareVersion,
		IPAddress,
		Port,
		InterfaceIPAddress,
		InterfacePort
	};
};
}  // namespace ots
#endif  // OTSDAQ_COMPONENTS_OTSDAQ_COMPONENTS_USERTABLEDATAFORMATS_FEPURDUENETWORKMASTERINTERFACETABLE_H_

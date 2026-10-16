#ifndef OTSDAQ_COMPONENTS_OTSDAQ_COMPONENTS_USERTABLEDATAFORMATS_FSSRDACSTABLE_H_
#define OTSDAQ_COMPONENTS_OTSDAQ_COMPONENTS_USERTABLEDATAFORMATS_FSSRDACSTABLE_H_

#include "otsdaq/TableDataFormats/DACsTableBase.h"

namespace ots
{
class FSSRDACsConfiguration : public DACsTableBase
{
  public:
	FSSRDACsConfiguration(void);
	virtual ~FSSRDACsConfiguration(void);

	/// Methods
	/// void init(ConfigurationManager *configManager);
	///
	/// Getters
	///
  private:
	enum
	{
		DetectorID,
		PulserData,
		PulserControl,
		IntegratorVbn,
		ShaperVbp2,
		ShaperVbp1,
		BLRestorer,
		VTn,
		VTp0,
		VTp1,
		VTp2,
		VTp3,
		VTp4,
		VTp5,
		VTp6,
		VTp7,
		ActiveLines,
		SendData,
		RejectHits,
		DigContrReg,
		AqBCO
	};
};
}  // namespace ots
#endif  // OTSDAQ_COMPONENTS_OTSDAQ_COMPONENTS_USERTABLEDATAFORMATS_FSSRDACSTABLE_H_

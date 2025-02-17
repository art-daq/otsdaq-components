#ifndef _ots_FEOtsUDPProducerTemplateInterface_h_
#define _ots_FEOtsUDPProducerTemplateInterface_h_

//#include "otsdaq/FECore/FEVInterface.h"
#include "otsdaq-components/DAQHardware/OtsUDPFirmwareDataGen.h"
#include "otsdaq-components/DAQHardware/OtsUDPHardware.h"
#include "otsdaq/DataManager/DataProducer.h"
#include "otsdaq/FECore/FEProducerVInterface.h"

#include <string>

namespace ots
{
/// public virtual inheritance only inherits the base class WorkLoop (and others) members
/// once)
class FEOtsUDPProducerTemplateInterface
    :  // public FEVInterface, public OtsUDPHardware, public OtsUDPFirmwareDataGen
       public FEProducerVInterface,
       public OtsUDPHardware,
       public OtsUDPFirmwareDataGen
{
  public:
	FEOtsUDPProducerTemplateInterface(const std::string&       interfaceUID,
	                                  const ConfigurationTree& theXDAQContextConfigTree,
	                                  const std::string& interfaceConfigurationPath);
	virtual ~FEOtsUDPProducerTemplateInterface(void);

	void configure(void) override;
	void halt(void) override;
	void pause(void) override;
	void resume(void) override;
	void start(std::string runNumber) override;
	void stop(void) override;
	bool running(void) override;

	virtual void universalRead(char* address, char* readValue) override;
	virtual void universalWrite(char* address, char* writeValue) override;

  private:
  public:  ///< FEMacro 'varTest' generated, Oct-11-2018 11:36:28, by 'admin' using
	       // MacroMaker.
	void varTest(__ARGS__);

  public:  ///< FEMacro 'varTest2' generated, Oct-11-2018 02:28:57, by 'admin' using
	       // MacroMaker.
	void varTest2(__ARGS__);
};
}  // namespace ots

#endif

#ifndef _ots_FEOtsUDPTemplateInterface_h_
#define _ots_FEOtsUDPTemplateInterface_h_

#include "otsdaq-components/DAQHardware/OtsUDPFirmwareDataGen.h"
#include "otsdaq-components/DAQHardware/OtsUDPHardware.h"

#include <string>
#include "otsdaq/FECore/FEVInterface.h"

namespace ots
{
class FEOtsUDPTemplateInterface : public FEVInterface,
                                  public OtsUDPHardware,
                                  public OtsUDPFirmwareDataGen
{
  public:
	FEOtsUDPTemplateInterface(const std::string&       interfaceUID,
	                          const ConfigurationTree& theXDAQContextConfigTree,
	                          const std::string&       interfaceConfigurationPath);
	virtual ~FEOtsUDPTemplateInterface(void);

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

  public
	  :  ///< FEMacro 'test' generated, Jan-20-2020 09:41:49, by 'admin' using MacroMaker.
	void test(__ARGS__);
};
}  // namespace ots

#endif

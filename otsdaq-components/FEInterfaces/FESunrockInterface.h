#ifndef _ots_FESunrockInterface_h_
#define _ots_FESunrockInterface_h_

#include <cstdint>
#include <map>
#include <mutex>
#include <string>
#include "otsdaq/FECore/FEVInterface.h"

namespace ots
{
/// Front-end interface for the Sunrock hardware, read from its Zynq FPGA over ssh.
///
/// FIRST VERSION: placeholders only. Nothing is sent to hardware yet; every Zynq
/// access goes through runZynqCommand(), which only logs the ssh command line it
/// would run and returns a simulated reply. Replace that one function with a real
/// ssh invocation to make monitoring, control and universalRead/Write live.
///
/// Placeholders for the three planned functions:
///  - monitoring:  pollStatus() / FE macro 'sunrockStatus' / running() poll loop
///  - control:     FE macro 'sunrockSetValue' and universalRead/universalWrite
///  - streaming:   startStreaming()/stopStreaming() (no data path yet)
class FESunrockInterface : public FEVInterface
{
  public:
	FESunrockInterface(const std::string&       interfaceUID,
	                   const ConfigurationTree& theXDAQContextConfigTree,
	                   const std::string&       interfaceConfigurationPath);
	virtual ~FESunrockInterface(void);

	void configure(void) override;
	void halt(void) override;
	void pause(void) override;
	void resume(void) override;
	void start(std::string runNumber) override;
	void stop(void) override;
	bool running(void) override;

	void universalRead(char* address, char* readValue) override;
	void universalWrite(char* address, char* writeValue) override;

	// FE macros (placeholders)
	void sunrockStatus(__ARGS__);
	void sunrockSetValue(__ARGS__);

  private:
	struct SunrockStatus
	{
		std::string connection  = "not polled";
		std::string firmware    = "unknown";
		double      temperature = 0.0;
	};

	std::string runZynqCommand(const std::string& command);  // PLACEHOLDER: no ssh yet
	std::string zynqSshCommandLine(const std::string& command) const;
	void        pollStatus(void);
	void        startStreaming(void);
	void        stopStreaming(void);

	// from the FESunrockInterfaceTable record
	std::string  zynqHostName_;
	std::string  zynqSshUser_;
	unsigned int zynqSshPort_;
	unsigned int commandTimeoutSeconds_;
	unsigned int statusPollPeriodSeconds_;

	std::mutex    statusMutex_;
	SunrockStatus lastStatus_;

	std::mutex                          simulatedMutex_;
	std::map<uint64_t, uint64_t>        simulatedRegisters_;
	std::map<std::string, std::string>  simulatedControlValues_;

	bool streamingEnabled_ = false;
};
}  // namespace ots

#endif

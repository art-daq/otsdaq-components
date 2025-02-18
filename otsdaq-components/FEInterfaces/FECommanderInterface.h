#ifndef _ots_FECommanderInterface_h_
#define _ots_FECommanderInterface_h_

#include "otsdaq/FECore/FEVInterface.h"
#include "otsdaq/NetworkUtilities/TransceiverSocket.h"

namespace ots
{
class FECommanderInterface : public FEVInterface, public TransceiverSocket
{
  public:
	FECommanderInterface(const std::string&       interfaceUID,
	                     const ConfigurationTree& theXDAQContextConfigTree,
	                     const std::string&       interfaceConfigurationPath);
	virtual ~FECommanderInterface(void);

	void configure(void);
	void halt(void);
	void pause(void);
	void resume(void);
	void start(std::string runNumber = "");
	void stop(void);
	//	bool running          (void);

	void universalRead(char* address, char* returnValue) override { __FE_SS__ << "Universal read not defined (addressPtr=" << (address?'1':'0') << "returnPtr=" << (returnValue?'1':'0') << __E__; __FE_SS_THROW__; }
	void universalWrite(char* address, char* writeValue) override { __FE_SS__ << "Universal write not defined (addressPtr=" << (address?'1':'0') << "writePtr=" << (writeValue?'1':'0') << __E__; __FE_SS_THROW__; }


	virtual std::string getStatusProgressDetail(void) override;  ///< overriding VStateMachine::getStatusProgressDetail

private:

	void        send					(std::string buffer);
	static void	AppStatusWorkLoop		(FECommanderInterface* fePtr);


	Socket      interfaceSocket_;
	std::string stateMachineName_;
	std::string configurationAlias_;
	bool		onlyRunTransitions_;
	bool		expectTransitionAck_;
	bool		monitorRemoteAppStatus_;
	std::mutex 	remoteAppStatusMutex_;
	std::string remoteAppStatus_;
	std::atomic<bool> halted_;

	bool		inRun_;
};
}

#endif
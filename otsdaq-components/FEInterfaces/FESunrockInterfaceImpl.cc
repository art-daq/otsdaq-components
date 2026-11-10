#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <sstream>
#include "otsdaq-components/FEInterfaces/FESunrockInterface.h"
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/MessageFacility/MessageFacility.h"

using namespace ots;

//==============================================================================
FESunrockInterface::FESunrockInterface(const std::string&       interfaceUID,
                                       const ConfigurationTree& theXDAQContextConfigTree,
                                       const std::string&       interfaceConfigurationPath)
    : FEVInterface(interfaceUID, theXDAQContextConfigTree, interfaceConfigurationPath)
{
	const ConfigurationTree typeNode =
	    theXDAQContextConfigTree.getNode(interfaceConfigurationPath);

	zynqHostName_            = typeNode.getNode("ZynqHostName").getValue<std::string>();
	zynqSshUser_             = typeNode.getNode("ZynqSshUser").getValue<std::string>();
	zynqSshPort_             = typeNode.getNode("ZynqSshPort").getValue<unsigned int>();
	commandTimeoutSeconds_   = typeNode.getNode("CommandTimeoutSeconds").getValue<unsigned int>();
	statusPollPeriodSeconds_ = typeNode.getNode("StatusPollPeriodSeconds").getValue<unsigned int>();

	universalAddressSize_ = 8;
	universalDataSize_    = 8;

	FEVInterface::registerFEMacroFunction(
	    "sunrockStatus",  // feMacroName
	    static_cast<FEVInterface::frontEndMacroFunction_t>(
	        &FESunrockInterface::sunrockStatus),  // feMacroFunction
	    std::vector<std::string>{},               // namesOfInputArgs
	    std::vector<std::string>{
	        "connection", "firmware", "temperatureC", "streaming"},  // namesOfOutputArgs
	    1);                                                          // requiredUserPermissions

	FEVInterface::registerFEMacroFunction(
	    "sunrockSetValue",  // feMacroName
	    static_cast<FEVInterface::frontEndMacroFunction_t>(
	        &FESunrockInterface::sunrockSetValue),  // feMacroFunction
	    std::vector<std::string>{"name", "value"},  // namesOfInputArgs
	    std::vector<std::string>{"result"},         // namesOfOutputArgs
	    1);                                         // requiredUserPermissions

	__FE_COUT__ << "Sunrock FE placeholder instantiated for Zynq " << zynqSshUser_ << "@"
	            << zynqHostName_ << ":" << zynqSshPort_ << " (no hardware access yet)"
	            << __E__;
}  // end constructor

//==============================================================================
FESunrockInterface::~FESunrockInterface(void) {}

//==============================================================================
/// PLACEHOLDER for the transport. The real version runs the ssh command line from
/// zynqSshCommandLine() (with commandTimeoutSeconds_) and returns its stdout, throwing
/// on failure/timeout. For now it only logs the command and returns a simulated reply.
std::string FESunrockInterface::runZynqCommand(const std::string& command)
{
	__FE_COUT__ << "[PLACEHOLDER] would run: " << zynqSshCommandLine(command) << __E__;
	return "PLACEHOLDER";
}  // end runZynqCommand()

//==============================================================================
std::string FESunrockInterface::zynqSshCommandLine(const std::string& command) const
{
	std::stringstream ss;
	ss << "ssh -o BatchMode=yes -o ConnectTimeout=" << commandTimeoutSeconds_ << " -p "
	   << zynqSshPort_ << " " << zynqSshUser_ << "@" << zynqHostName_ << " '" << command
	   << "'";
	return ss.str();
}  // end zynqSshCommandLine()

//==============================================================================
/// PLACEHOLDER monitoring: refreshes the cached status with simulated values.
void FESunrockInterface::pollStatus(void)
{
	const std::string reply = runZynqCommand("sunrock_status");

	std::lock_guard<std::mutex> lock(statusMutex_);
	lastStatus_.connection  = "placeholder (" + reply + ")";
	lastStatus_.firmware    = "0.0.0-placeholder";
	lastStatus_.temperature = 40.0;
}  // end pollStatus()

//==============================================================================
/// PLACEHOLDER streaming: there is no data path yet. The real version needs a
/// producer-type interface (see FEOtsUDPProducerTemplateInterface) plus a data
/// buffer configured in this context, or a listener fed by the Zynq.
void FESunrockInterface::startStreaming(void)
{
	streamingEnabled_ = true;
	__FE_COUT__ << "[PLACEHOLDER] streaming started (no data is produced)" << __E__;
}  // end startStreaming()

//==============================================================================
void FESunrockInterface::stopStreaming(void)
{
	streamingEnabled_ = false;
	__FE_COUT__ << "[PLACEHOLDER] streaming stopped" << __E__;
}  // end stopStreaming()

//==============================================================================
void FESunrockInterface::configure(void)
{
	__FE_COUT__ << "configure" << __E__;
	pollStatus();
	__FE_COUT__ << "Done with Sunrock placeholder configuring." << __E__;
}  // end configure()

//==============================================================================
void FESunrockInterface::halt(void)
{
	__FE_COUT__ << "\tHalt" << __E__;
	stop();
}

//==============================================================================
void FESunrockInterface::pause(void)
{
	__FE_COUT__ << "\tPause" << __E__;
	stop();
}

//==============================================================================
void FESunrockInterface::resume(void)
{
	__FE_COUT__ << "\tResume" << __E__;
	start("");
}

//==============================================================================
void FESunrockInterface::start(std::string)  // runNumber)
{
	__FE_COUT__ << "\tStart" << __E__;
	startStreaming();
}

//==============================================================================
void FESunrockInterface::stop(void)
{
	__FE_COUT__ << "\tStop" << __E__;
	stopStreaming();
}

//==============================================================================
/// Monitoring loop: polls the (placeholder) status every statusPollPeriodSeconds_
/// while running, sleeping in short slices so stop/halt stay responsive.
bool FESunrockInterface::running(void)
{
	const unsigned int slices =
	    10 * (statusPollPeriodSeconds_ ? statusPollPeriodSeconds_ : 1);

	while(WorkLoop::continueWorkLoop_)
	{
		pollStatus();
		for(unsigned int i = 0; i < slices && WorkLoop::continueWorkLoop_; ++i)
			usleep(100000);
	}
	return false;
}  // end running()

//==============================================================================
/// universalRead
///	PLACEHOLDER: reads from a simulated register map (unwritten registers read 0).
///	address and returnValue are universalAddressSize_/universalDataSize_ (8) byte buffers.
void FESunrockInterface::universalRead(char* address, char* returnValue)
{
	uint64_t addr = 0;
	std::memcpy(&addr, address, std::min<size_t>(universalAddressSize_, sizeof(addr)));

	uint64_t value = 0;
	{
		std::lock_guard<std::mutex> lock(simulatedMutex_);
		auto                        it = simulatedRegisters_.find(addr);
		if(it != simulatedRegisters_.end())
			value = it->second;
	}

	std::stringstream ss;
	ss << "devmem 0x" << std::hex << addr;
	runZynqCommand(ss.str());

	std::memcpy(returnValue, &value, std::min<size_t>(universalDataSize_, sizeof(value)));
}  // end universalRead()

//==============================================================================
/// universalWrite
///	PLACEHOLDER: stores into a simulated register map.
void FESunrockInterface::universalWrite(char* address, char* writeValue)
{
	uint64_t addr = 0;
	std::memcpy(&addr, address, std::min<size_t>(universalAddressSize_, sizeof(addr)));
	uint64_t value = 0;
	std::memcpy(&value, writeValue, std::min<size_t>(universalDataSize_, sizeof(value)));

	{
		std::lock_guard<std::mutex> lock(simulatedMutex_);
		simulatedRegisters_[addr] = value;
	}

	std::stringstream ss;
	ss << "devmem 0x" << std::hex << addr << " 64 0x" << value;
	runZynqCommand(ss.str());
}  // end universalWrite()

//==============================================================================
/// sunrockStatus
///	PLACEHOLDER FE macro: refreshes and returns the cached (simulated) status.
void FESunrockInterface::sunrockStatus(__ARGS__)
{
	pollStatus();

	std::lock_guard<std::mutex> lock(statusMutex_);
	__SET_ARG_OUT__("connection", lastStatus_.connection);
	__SET_ARG_OUT__("firmware", lastStatus_.firmware);
	__SET_ARG_OUT__("temperatureC", lastStatus_.temperature);
	__SET_ARG_OUT__("streaming", std::string(streamingEnabled_ ? "on" : "off"));
}  // end sunrockStatus()

//==============================================================================
/// sunrockSetValue
///	PLACEHOLDER FE macro: records a named control value and logs the Zynq command.
void FESunrockInterface::sunrockSetValue(__ARGS__)
{
	const std::string& name  = FEVInterface::getFEMacroConstArgument(argsIn, "name");
	const std::string& value = FEVInterface::getFEMacroConstArgument(argsIn, "value");

	{
		std::lock_guard<std::mutex> lock(simulatedMutex_);
		simulatedControlValues_[name] = value;
	}

	const std::string reply = runZynqCommand("sunrock_set " + name + " " + value);
	__SET_ARG_OUT__("result", reply);
}  // end sunrockSetValue()

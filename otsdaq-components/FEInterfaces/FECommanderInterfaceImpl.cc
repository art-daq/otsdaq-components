#include "otsdaq-components/FEInterfaces/FECommanderInterface.h"
// #include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/Macros/InterfacePluginMacros.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>  //for memcpy
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

using namespace ots;

//========================================================================================================================
ots::FECommanderInterface::FECommanderInterface(
    const std::string&       interfaceUID,
    const ConfigurationTree& theXDAQContextConfigTree,
    const std::string&       interfaceConfigurationPath)
    : Socket(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                 .getNode("HostIPAddress")
                 .getValue<std::string>(),
             theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                 .getNode("HostPort")
                 .getValue<unsigned int>())
    , FEVInterface(interfaceUID, theXDAQContextConfigTree, interfaceConfigurationPath)
    , interfaceSocket_(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                           .getNode("RemoteInterfaceIPAddress")
                           .getValue<std::string>(),
                       theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                           .getNode("RemoteInterfacePort")
                           .getValue<unsigned int>())
    , stateMachineName_(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                            .getNode("StateMachineName")
                            .getValue<std::string>())
    , configurationAlias_(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                              .getNode("ConfigurationAlias")
                              .getValue<std::string>())
    , onlyRunTransitions_(false)
    , monitorRemoteAppStatus_(false)
    , halted_(false)
    , inRun_(false)
{
	Socket::initialize();
	universalAddressSize_ = 8;
	universalDataSize_    = 8;

	try
	{
		onlyRunTransitions_ = theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
		                          .getNode("OnlyRunTransitions")
		                          .getValue<bool>();
	}
	catch(...)
	{
		;
	}  //ignore missing setting

	__COUTV__(onlyRunTransitions_);

	try
	{
		monitorRemoteAppStatus_ =
		    theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
		        .getNode("MonitorRemoteAppStatus")
		        .getValue<bool>();
	}
	catch(...)
	{
		;
	}  //ignore missing setting

	__COUTV__(monitorRemoteAppStatus_);

	try
	{
		expectTransitionAck_ =
		    theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
		        .getNode("ExpectTransitionAck")
		        .getValue<bool>();
	}
	catch(...)
	{
		;
	}  //ignore missing setting

	__COUTV__(expectTransitionAck_);

	if(monitorRemoteAppStatus_)
	{
		__COUT__ << "Enabling App Status checking..." << __E__;
		std::thread(
		    [](FECommanderInterface* s) { FECommanderInterface::AppStatusWorkLoop(s); },
		    this)
		    .detach();
	}
	else
	{
		__COUT__ << "App Status checking is disabled." << __E__;

		// set all app status to "Not Monitored"
		std::lock_guard<std::mutex> lock(remoteAppStatusMutex_);
		remoteAppStatus_ = SupervisorInfo::APP_STATUS_NOT_MONITORED;
	}

}  //end constructor()

//========================================================================================================================
ots::FECommanderInterface::~FECommanderInterface(void)
{
	__COUT__ << "Destructor" << __E__;
}  //end destructor()

//==============================================================================
/// AppStatusWorkLoop
///	child thread
void FECommanderInterface::AppStatusWorkLoop(FECommanderInterface* fePtr)
{
	__COUT__ << "Started remote status checking loop..." << __E__;
	std::string status;
	bool        verbose = true;
	while(!fePtr->halted_)
	{
		sleep(1);

		try
		{
			status = fePtr->sendAndReceive(fePtr->interfaceSocket_,
			                               "GetRemoteAppStatus",
			                               3 /* timeout seconds */,
			                               0 /* timeout microseconds */,
			                               verbose);
		}
		catch(const std::exception& e)
		{
			__COUT_WARN__ << "No remote app status received. Trying to ignore." << __E__;
			sleep(5);
			continue;
		}

		__COUTT__ << "Remote app status: " << status << __E__;

		std::lock_guard<std::mutex> lock(fePtr->remoteAppStatusMutex_);
		fePtr->remoteAppStatus_ = "Remote:" + status;
	}  // end of infinite status checking loop
	__COUT__ << "Exited remote status checking loop." << __E__;
}  // end AppStatusWorkLoop()

//==============================================================================
/// virtual progress string that can be overridden with more info
///	e.g. steps and substeps
///	however integer 0-100 should be first number, then separated by : colons
///	e.g. 94:FE0:1:2
std::string FECommanderInterface::getStatusProgressDetail(void)
{
	std::lock_guard<std::mutex> lock(remoteAppStatusMutex_);
	return remoteAppStatus_;
}  // end getStatusProgressString()

//========================================================================================================================
void ots::FECommanderInterface::send(std::string buffer)
{
	try
	{
		bool verbose = false;
		__FE_COUT__ << "Sending: '" << buffer << "' "
		            << (expectTransitionAck_ ? "and waiting for Ack" : "") << std::endl;

		if(!expectTransitionAck_)
		{
			if(TransceiverSocket::send(interfaceSocket_, buffer, verbose) < 0)
			{
				__FE_SS__ << "Send failed to IP:Port " << interfaceSocket_.getIPAddress()
				          << ":" << interfaceSocket_.getPort() << __E__;
				__FE_SS_THROW__;
			}
		}
		else  //expectTransitionAck_
		{
			std::string response =
			    TransceiverSocket::sendAndReceive(interfaceSocket_,
			                                      buffer,
			                                      5 /* timeout seconds */,
			                                      0 /* timeout microseconds */,
			                                      verbose);

			if("Done" != response)
			{
				__FE_SS__ << "Send-and-Receive failed to remote IP:Port "
				          << interfaceSocket_.getIPAddress() << ":"
				          << interfaceSocket_.getPort()
				          << ". Here is response received from the remote target = \n'"
				          << response << "\n'... - expecting 'Done.'" << __E__;
				__FE_SS_THROW__;
			}
		}
	}
	catch(...)
	{
		__FE_SS__ << "Failed to send command '" << buffer
		          << "' to remote state machine. ";
		try
		{
			throw;
		}
		catch(const std::exception& e)
		{
			ss << "Here is the error: " << e.what() << __E__;
		}
		catch(...)
		{
			ss << "Unrecognized exception caught!" << __E__;
		}

		__FE_SS_THROW__;
	}
}  //end send()

//========================================================================================================================
void ots::FECommanderInterface::halt(void)
{
	halted_ = true;
	if(onlyRunTransitions_)
	{
		__FE_COUT__ << "Only executing run transitions - skipping Halt transition."
		            << __E__;
		return;
	}
	// MESSAGE = "PhysicsRuns0,Halt"

	__FE_COUTV__(inRun_);
	if(inRun_)
		send(stateMachineName_ + ",Abort");
	else
		send(stateMachineName_ + ",Halt");
}  //end halt()

//========================================================================================================================
void ots::FECommanderInterface::pause(void) { send(stateMachineName_ + ",Pause"); }

//========================================================================================================================
void ots::FECommanderInterface::resume(void) { send(stateMachineName_ + ",Resume"); }
//========================================================================================================================
void ots::FECommanderInterface::start(std::string runNumber)
{
	// MESSAGE = "PhysicsRuns0,Start, %i" % (int(run)) #"PhysicsRuns0,Start"
	send(stateMachineName_ + ",Start," + runNumber);
	inRun_ = true;  //track to control Abort vs Halt
}  //end start()

//========================================================================================================================
void ots::FECommanderInterface::stop(void)
{
	// MESSAGE = "PhysicsRuns0,Stop"
	send(stateMachineName_ + ",Stop");
	inRun_ = false;  //track to control Abort vs Halt
}  //end stop()

//========================================================================================================================
void ots::FECommanderInterface::configure(void)
{
	if(onlyRunTransitions_)
	{
		__FE_COUT__ << "Only executing run transitions - skipping Configure transition."
		            << __E__;
		return;
	}

	__FE_COUT__ << "Configure" << __E__;
	// MESSAGE = "PhysicsRuns0,Configure,FQNETConfig"
	send(stateMachineName_ + ",Configure," + configurationAlias_);
}  //end configure()
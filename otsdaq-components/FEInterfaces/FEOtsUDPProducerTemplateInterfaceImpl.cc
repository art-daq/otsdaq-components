#include <iostream>
#include <set>
#include "otsdaq-components/FEInterfaces/FEOtsUDPProducerTemplateInterface.h"
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/MessageFacility/MessageFacility.h"

using namespace ots;

//==============================================================================
FEOtsUDPProducerTemplateInterface::FEOtsUDPProducerTemplateInterface(
    const std::string&       interfaceUID,
    const ConfigurationTree& theXDAQContextConfigTree,
    const std::string&       interfaceConfigurationPath)
    : Socket(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                 .getNode("HostIPAddress")
                 .getValue<std::string>(),
             theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                 .getNode("HostPort")
                 .getValue<unsigned int>())
    //, WorkLoop (interfaceUID + "-workloop")
    , FEProducerVInterface(
          interfaceUID, theXDAQContextConfigTree, interfaceConfigurationPath)
    //, DataProducer ("supervisorID","bufferID","processorID",100/*bufferSize*/)
    , OtsUDPHardware(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                         .getNode("InterfaceIPAddress")
                         .getValue<std::string>(),
                     theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                         .getNode("InterfacePort")
                         .getValue<unsigned int>())
    , OtsUDPFirmwareDataGen(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                                .getNode("FirmwareVersion")
                                .getValue<unsigned int>())
{
	// registration of FEMacro 'varTest2' generated, Oct-11-2018 02:28:57, by 'admin'
	// using MacroMaker.
	FEVInterface::registerFEMacroFunction(
	    "varTest2",  // feMacroName
	    static_cast<FEVInterface::frontEndMacroFunction_t>(
	        &FEOtsUDPProducerTemplateInterface::varTest2),  // feMacroFunction
	    std::vector<std::string>{"myOtherArg"},             // namesOfInputArgs
	    std::vector<std::string>{"myArg", "outArg1"},       // namesOfOutputArgs
	    1);                                                 // requiredUserPermissions

	// registration of FEMacro 'varTest' generated, Oct-11-2018 11:36:28, by 'admin' using
	// MacroMaker.
	FEVInterface::registerFEMacroFunction(
	    "varTest",  // feMacroName
	    static_cast<FEVInterface::frontEndMacroFunction_t>(
	        &FEOtsUDPProducerTemplateInterface::varTest),  // feMacroFunction
	    std::vector<std::string>{"myOtherArg"},            // namesOfInputArgs
	    std::vector<std::string>{"myArg", "outArg1"},      // namesOfOutputArgs
	    1);                                                // requiredUserPermissions

	universalAddressSize_ = 8;
	universalDataSize_    = 8;
}

//==============================================================================
FEOtsUDPProducerTemplateInterface::~FEOtsUDPProducerTemplateInterface(void) {}

//==============================================================================
void FEOtsUDPProducerTemplateInterface::configure(void)
{
	//	unsigned int i = VStateMachine::getIterationIndex();
	//	unsigned int j = VStateMachine::getSubIterationIndex();
	//	if(i == 0 && j < 5)
	//		VStateMachine::indicateSubIterationWork();
	//	else if(i < 10)
	//		VStateMachine::indicateIterationWork();
	//
	//	__FE_COUTV__(VStateMachine::getSubIterationIndex());
	//	__FE_COUTV__(VStateMachine::getSubIterationWork());
	//	__FE_COUTV__(VStateMachine::getIterationIndex());
	//	__FE_COUTV__(VStateMachine::getIterationWork());
	//
	//	__FE_COUTV__(i);
	//	__FE_COUTV__(getInterfaceUID());
	//	switch(i)
	//	{
	//	case 1:
	//		if(getInterfaceUID() == "ExampleInterface0")
	//		{
	//			FEVInterface::sendToFrontEnd("ExampleInterface1","Hello1");
	//			FEVInterface::sendToFrontEnd("ExampleInterface1",4.3f);
	//			FEVInterface::sendToFrontEnd("ExampleInterface1",4);
	//			FEVInterface::sendToFrontEnd("ExampleInterface2","Hello2");
	//			FEVInterface::sendToFrontEnd("ExampleInterface2",4.6f);
	//			FEVInterface::sendToFrontEnd("ExampleInterface2",6);
	//		}
	//		break;
	//	case 2:
	//		if(getInterfaceUID() != "ExampleInterface0")
	//		{
	//			std::string a = FEVInterface::receiveFromFrontEnd("ExampleInterface0");
	//			double b = FEVInterface::receiveFromFrontEnd<double>("ExampleInterface0");
	//			unsigned short c =
	//					FEVInterface::receiveFromFrontEnd<unsigned
	// short>("ExampleInterface0");
	//			__FE_COUTV__(a);
	//			__FE_COUTV__(b);
	//			__FE_COUTV__(c);
	//		}
	//		break;
	//	case 3:
	//		if(getInterfaceUID() == "ExampleInterface0")
	//		{
	//			std::vector<frontEndMacroArg_t> argsIn;
	//			__SET_ARG_IN__("myOtherArg",(unsigned int)5);
	//
	//			std::vector<frontEndMacroArg_t> argsOut;
	//
	//			__FE_COUTV__(StringMacros::vectorToString(argsIn));
	//
	//			FEVInterface::runFrontEndMacro("ExampleInterface2","varTest2",
	//					argsIn,argsOut);
	//
	//			__FE_COUTV__(StringMacros::vectorToString(argsOut));
	//			__FE_COUTV__(FEVInterface::getFEMacroArgument(
	//					argsOut,"outArg1"));
	//
	//			{
	//				std::string a = FEVInterface::getFEMacroArgument(
	//						argsOut,"myArg");
	//				double b = getFEMacroArgumentValue<double>(
	//						argsOut,"outArg1");
	//				unsigned short c = getFEMacroArgumentValue<unsigned short>(
	//						argsOut,"outArg1");
	//				__FE_COUTV__(a);
	//				__FE_COUTV__(b);
	//				__FE_COUTV__(c);
	//			}
	//
	//
	//
	//			FEVInterface::runFrontEndMacro("ExampleInterface1","varTest",
	//					argsIn,argsOut);
	//			__FE_COUTV__(StringMacros::vectorToString(argsOut));
	//
	//			{
	//				std::string a = __GET_ARG_OUT__("myArg",std::string);
	//				double b = __GET_ARG_OUT__("outArg1",double);
	//				unsigned short c = __GET_ARG_OUT__("outArg1",unsigned short);
	//				__FE_COUTV__(a);
	//				__FE_COUTV__(b);
	//				__FE_COUTV__(c);
	//			}
	//		}
	//
	//		break;
	//	default:;
	//	}
	//
	//
	//	return;

	__FE_COUT__ << "configure" << __E__;
	__FE_COUT__ << "Clearing receive socket buffer: " << OtsUDPHardware::clearReadSocket()
	            << " packets cleared." << __E__;

	std::string sendBuffer;
	std::string recvBuffer;
	uint64_t    readQuadWord;

	__FE_COUT__
	    << "Configuration Path Table: "
	    << theXDAQContextConfigTree_.getNode(theConfigurationPath_).getTableName() << "-v"
	    << theXDAQContextConfigTree_.getNode(theConfigurationPath_).getTableVersion()
	    << __E__;

	__FE_COUT__ << "Interface name: "
	            << theXDAQContextConfigTree_.getNode(theConfigurationPath_) << __E__;

	__FE_COUT__ << "Configured Firmware Version: "
	            << theXDAQContextConfigTree_.getNode(theConfigurationPath_)
	                   .getNode("FirmwareVersion")
	                   .getValue<unsigned int>()
	            << __E__;

	__FE_COUT__ << "Setting Destination IP: "
	            << theXDAQContextConfigTree_.getNode(theConfigurationPath_)
	                   .getNode("StreamToIPAddress")
	                   .getValue<std::string>()
	            << __E__;
	__FE_COUT__ << "And Destination Port: "
	            << theXDAQContextConfigTree_.getNode(theConfigurationPath_)
	                   .getNode("StreamToPort")
	                   .getValue<unsigned int>()
	            << __E__;

	OtsUDPFirmwareCore::setDataDestination(
	    sendBuffer,
	    theXDAQContextConfigTree_.getNode(theConfigurationPath_)
	        .getNode("StreamToIPAddress")
	        .getValue<std::string>(),
	    theXDAQContextConfigTree_.getNode(theConfigurationPath_)
	        .getNode("StreamToPort")
	        .getValue<uint64_t>());
	OtsUDPHardware::write(sendBuffer);

	__FE_COUT__ << "Reading back burst dest MAC/IP/Port: " << __E__;

	OtsUDPFirmwareCore::readDataDestinationMAC(sendBuffer);
	OtsUDPHardware::read(sendBuffer, readQuadWord);

	OtsUDPFirmwareCore::readDataDestinationIP(sendBuffer);
	OtsUDPHardware::read(sendBuffer, readQuadWord);

	OtsUDPFirmwareCore::readDataDestinationPort(sendBuffer);
	OtsUDPHardware::read(sendBuffer, readQuadWord);

	OtsUDPFirmwareCore::readControlDestinationPort(sendBuffer);
	OtsUDPHardware::read(sendBuffer, readQuadWord);

	// Run Configure Sequence Commands
	FEVInterface::runSequenceOfCommands("LinkToConfigureSequence");

	__FE_COUT__ << "Done with ots Template configuring." << __E__;
}

//==============================================================================
/// void FEOtsUDPProducerTemplateInterface::configureDetector(const DACStream&
/// theDACStream)
//{
//	__FE_COUT__ << "\tconfigureDetector" << __E__;
//}

//==============================================================================
void FEOtsUDPProducerTemplateInterface::halt(void)
{
	__FE_COUT__ << "\tHalt" << __E__;
	stop();
}

//==============================================================================
void FEOtsUDPProducerTemplateInterface::pause(void)
{
	__FE_COUT__ << "\tPause" << __E__;
	stop();
}

//==============================================================================
void FEOtsUDPProducerTemplateInterface::resume(void)
{
	__FE_COUT__ << "\tResume" << __E__;
	start("");
}

//==============================================================================
void FEOtsUDPProducerTemplateInterface::start(std::string)  // runNumber)
{
	__FE_COUT__ << "\tStart" << __E__;

	//		unsigned int i = VStateMachine::getIterationIndex();
	//		unsigned int j = VStateMachine::getSubIterationIndex();
	//		if(i == 0 && j < 5)
	//			VStateMachine::indicateSubIterationWork();
	//		else if(i < 10)
	//			VStateMachine::indicateIterationWork();
	//
	//
	//		__FE_COUTV__(VStateMachine::getSubIterationIndex());
	//		__FE_COUTV__(VStateMachine::getSubIterationWork());
	//		__FE_COUTV__(VStateMachine::getIterationIndex());
	//		__FE_COUTV__(VStateMachine::getIterationWork());
	//
	//
	//		return;

	// Run Start Sequence Commands
	FEVInterface::runSequenceOfCommands("LinkToStartSequence");

	std::string sendBuffer;
	OtsUDPFirmwareCore::startBurst(sendBuffer);
	OtsUDPHardware::write(sendBuffer);
}

//==============================================================================
void FEOtsUDPProducerTemplateInterface::stop(void)
{
	__FE_COUT__ << "\tStop" << __E__;

	// Run Stop Sequence Commands

	FEVInterface::runSequenceOfCommands("LinkToStopSequence");

	std::string sendBuffer;
	OtsUDPFirmwareCore::stopBurst(sendBuffer);
	OtsUDPHardware::write(sendBuffer);
}

//==============================================================================
bool FEOtsUDPProducerTemplateInterface::running(void)
{
	__FE_COUT__ << "\tRunning" << __E__;

	//__SS__ << "?" << __E__; //test exceptions during running
	//__SS_THROW__;

	int state = -1;
	while(WorkLoop::continueWorkLoop_)
	{
		// while running
		// play with the LEDs at address 0x1003

		++state;
		if(state < 8)
			sleep(1);
		else
		{
			//			if(1 || getInterfaceUID() == "ExampleInterface1")
			//			{
			//				throw __OTS_PAUSE_EXCEPTION__("Soft error here");
			//			}
			//			else
			break;
		}

		{
			if(DataProducerBase::attachToEmptySubBuffer(dataP_, headerP_) < 0)
			{
				__CFG_COUT__ << "There are no available buffers! Retrying...after "
				                "waiting 10 milliseconds!"
				             << std::endl;
				usleep(10000);
				return true;
			}

			if(1)  // test data buffers
			{
				// sleep(1);
				unsigned long long value  = 0xB57321;  // this is 8-bytes
				std::string&       buffer = *dataP_;
				buffer.resize(8);  // NOTE: this is inexpensive according to
				                   // Lorenzo/documentation in C++11 (only increases size
				                   // once and doesn't decrease size)
				memcpy(
				    (void*)&buffer[0] /*dest*/, (void*)&value /*src*/, 8 /*numOfBytes*/);

				// size() and length() are equivalent
				__CFG_COUT__ << "Writing to buffer " << buffer.size() << " bytes!"
				             << __E__;
				__CFG_COUT__ << "Writing to buffer length " << buffer.length()
				             << " bytes!" << __E__;

				__CFG_COUT__ << "Buffer Data: "
				             << BinaryStringMacros::binaryNumberToHexString(buffer)
				             << __E__;

				__CFG_COUTV__(DataProcessor::theCircularBuffer_);

				__CFG_COUT__ << __E__;

				__CFG_COUT__ << __E__;

				DataProducerBase::setWrittenSubBuffer<
				    std::string,
				    std::map<std::string, std::string> >();
				__CFG_COUT__ << __E__;
				__CFG_COUTV__(DataProcessor::theCircularBuffer_);
			}
		}
		if(0)
		{
			unsigned long long value = 0xA54321;  // this is 8-bytes
			std::string        buffer;
			buffer.resize(8);  // NOTE: this is inexpensive according to
			                   // Lorenzo/documentation in C++11 (only increases size once
			                   // and doesn't decrease size)
			memcpy((void*)&buffer[0] /*dest*/, (void*)&value /*src*/, 8 /*numOfBytes*/);

			// size() and length() are equivalent
			__FE_COUT__ << "Writing to buffer " << buffer.size() << " bytes!" << __E__;
			__FE_COUT__ << "Writing to buffer length " << buffer.length() << " bytes!"
			            << __E__;

			__FE_COUT__ << "Buffer Data: "
			            << BinaryStringMacros::binaryNumberToHexString(buffer) << __E__;

			FEProducerVInterface::copyToNextBuffer(buffer);
		}
	}

	//		//example!
	//		//play with array of 8 LEDs at address 0x1003

	//
	//	bool flashLEDsWhileRunning = false;
	//	if(flashLEDsWhileRunning)
	//	{
	//		std::string writeBuffer;
	//		int state = -1;
	//		while(WorkLoop::continueWorkLoop_)
	//		{
	//			//while running
	//			//play with the LEDs at address 0x1003
	//
	//			++state;
	//			if(state < 8)
	//			{
	//				writeBuffer.resize(0);
	//				OtsUDPFirmwareCore::write(writeBuffer, 0x1003,1<<state);
	//				OtsUDPHardware::write(writeBuffer);
	//			}
	//			else if(state%2 == 1 && state < 11)
	//			{
	//				writeBuffer.resize(0);
	//				OtsUDPFirmwareCore::write(writeBuffer, 0x1003, 0xFF);
	//				OtsUDPHardware::write(writeBuffer);
	//			}
	//			else if(state%2 == 0 && state < 11)
	//			{
	//				writeBuffer.resize(0);
	//				OtsUDPFirmwareCore::write(writeBuffer, 0x1003,0);
	//				OtsUDPHardware::write(writeBuffer);
	//			}
	//			else
	//				state = -1;
	//
	//			sleep(1);
	//		}
	//	}

	return false;
}

//==============================================================================
/// NOTE: buffer for address must be at least size universalAddressSize_
/// NOTE: buffer for returnValue must be max UDP size to handle return possibility
void ots::FEOtsUDPProducerTemplateInterface::universalRead(char* address,
                                                           char* returnValue)
{
	__FE_COUT__ << "address size " << universalAddressSize_ << __E__;

	__FE_COUT__ << "Request: ";
	for(unsigned int i = 0; i < universalAddressSize_; ++i)
		printf("%2.2X", (unsigned char)address[i]);
	std::cout << __E__;

	std::string readBuffer, sendBuffer;
	OtsUDPFirmwareCore::readAdvanced(sendBuffer, address, 1 /*size*/);

	OtsUDPHardware::read(sendBuffer, readBuffer);  // data reply

	__FE_COUT__ << "Result SIZE: " << readBuffer.size() << __E__;
	std::memcpy(returnValue, readBuffer.substr(2).c_str(), universalDataSize_);
}  // end universalRead()

//==============================================================================
/// NOTE: buffer for address must be at least size universalAddressSize_
/// NOTE: buffer for writeValue must be at least size universalDataSize_
void ots::FEOtsUDPProducerTemplateInterface::universalWrite(char* address,
                                                            char* writeValue)
{
	__FE_COUT__ << "address size " << universalAddressSize_ << __E__;
	__FE_COUT__ << "data size " << universalDataSize_ << __E__;
	__FE_COUT__ << "Sending: ";
	for(unsigned int i = 0; i < universalAddressSize_; ++i)
		printf("%2.2X", (unsigned char)address[i]);
	std::cout << __E__;

	std::string sendBuffer;
	OtsUDPFirmwareCore::writeAdvanced(sendBuffer, address, writeValue, 1 /*size*/);
	OtsUDPHardware::write(sendBuffer);  // data request
}  // end universalWrite()

//==============================================================================
/// varTest
///	FEMacro 'varTest' generated, Oct-11-2018 11:36:28, by 'admin' using MacroMaker.
///	Macro Notes: This is a great test!
void FEOtsUDPProducerTemplateInterface::varTest(__ARGS__)
{
	__FE_COUT__ << "# of input args = " << argsIn.size() << __E__;
	__FE_COUT__ << "# of output args = " << argsOut.size() << __E__;
	for(auto& argIn : argsIn)
		__FE_COUT__ << argIn.first << ": " << argIn.second << __E__;

	// macro commands section
	{
		char* address = new char[universalAddressSize_]{
		    0};  // create address buffer of interface size and init to all 0
		char* data = new char[universalDataSize_]{
		    0};                 // create data buffer of interface size and init to all 0
		uint64_t macroAddress;  // create macro address buffer (size 8 bytes)
		// uint64_t macroData;     // create macro address buffer (size 8 bytes)
		std::map<std::string /*arg name*/, uint64_t /*arg val*/>
		    macroArgs;  // create map from arg name to 64-bit number

		// command-#0: Read(0x1002 /*address*/,myArg /*data*/);
		macroAddress = 0x1002;
		memcpy(address, &macroAddress, 8);  // copy macro address to buffer
		universalRead(address, data);
		memcpy(&macroArgs["myArg"], data, 8);          // copy buffer to argument map
		__SET_ARG_OUT__("myArg", macroArgs["myArg"]);  // update output argument result

		// command-#1: Read(0x1001 /*address*/,data);
		macroAddress = 0x1001;
		memcpy(address, &macroAddress, 8);  // copy macro address to buffer
		universalRead(address, data);
		memcpy(&macroArgs["outArg1"], data, 8);            // copy buffer to argument map
		__SET_ARG_OUT__("outArg1", macroArgs["outArg1"]);  // update output argument
		                                                   // result

		// command-#2: Write(0x1002 /*address*/,myOtherArg /*data*/);
		macroAddress = 0x1002;
		memcpy(address, &macroAddress, 8);  // copy macro address to buffer
		macroArgs["myOtherArg"] = __GET_ARG_IN__(
		    "myOtherArg",
		    uint64_t);  // initialize from input arguments	//get macro data argument
		memcpy(data, &macroArgs["myOtherArg"], 8);  // copy macro data argument to buffer
		universalWrite(address, data);

		// command-#3: Write(0x1001 /*address*/,myArg /*data*/);
		macroAddress = 0x1001;
		memcpy(address,
		       &macroAddress,
		       8);  // copy macro address to buffer	//get macro data argument
		memcpy(data, &macroArgs["myArg"], 8);  // copy macro data argument to buffer
		universalWrite(address, data);

		// command-#4: delay(4);
		__FE_COUT__ << "Sleeping for... " << 4 << " milliseconds " << __E__;
		usleep(4 * 1000 /* microseconds */);

		delete[] address;  // free the memory
		delete[] data;     // free the memory
	}

	for(auto& argOut : argsOut)
		__FE_COUT__ << argOut.first << ": " << argOut.second << __E__;

}  // end varTest()

//==============================================================================
/// varTest2
///	FEMacro 'varTest2' generated, Oct-11-2018 02:28:57, by 'admin' using MacroMaker.
///	Macro Notes: [Modified 14:28 10/11/2018] This is a great test!
void FEOtsUDPProducerTemplateInterface::varTest2(__ARGS__)
{
	__FE_COUT__ << "# of input args = " << argsIn.size() << __E__;
	__FE_COUT__ << "# of output args = " << argsOut.size() << __E__;
	for(auto& argIn : argsIn)
		__FE_COUT__ << argIn.first << ": " << argIn.second << __E__;

	// macro commands section
	{
		char* address = new char[universalAddressSize_]{
		    0};  // create address buffer of interface size and init to all 0
		char* data = new char[universalDataSize_]{
		    0};                 // create data buffer of interface size and init to all 0
		uint64_t macroAddress;  // create macro address buffer (size 8 bytes)
		// uint64_t macroData;     // create macro address buffer (size 8 bytes)
		std::map<std::string /*arg name*/, uint64_t /*arg val*/>
		    macroArgs;  // create map from arg name to 64-bit number

		// command-#0: Read(0x1002 /*address*/,myArg /*data*/);
		macroAddress = 0x1002;
		memcpy(address, &macroAddress, 8);  // copy macro address to buffer
		universalRead(address, data);
		memcpy(&macroArgs["myArg"], data, 8);          // copy buffer to argument map
		__SET_ARG_OUT__("myArg", macroArgs["myArg"]);  // update output argument result

		// command-#1: Read(0x1001 /*address*/,data);
		macroAddress = 0x1001;
		memcpy(address, &macroAddress, 8);  // copy macro address to buffer
		universalRead(address, data);
		memcpy(&macroArgs["outArg1"], data, 8);            // copy buffer to argument map
		__SET_ARG_OUT__("outArg1", macroArgs["outArg1"]);  // update output argument
		                                                   // result

		// command-#2: Write(0x1002 /*address*/,myOtherArg /*data*/);
		macroAddress = 0x1002;
		memcpy(address, &macroAddress, 8);  // copy macro address to buffer
		macroArgs["myOtherArg"] = __GET_ARG_IN__(
		    "myOtherArg",
		    uint64_t);  // initialize from input arguments	//get macro data argument
		memcpy(data, &macroArgs["myOtherArg"], 8);  // copy macro data argument to buffer
		universalWrite(address, data);

		// command-#3: Write(0x1001 /*address*/,myArg /*data*/);
		macroAddress = 0x1001;
		memcpy(address,
		       &macroAddress,
		       8);  // copy macro address to buffer	//get macro data argument
		memcpy(data, &macroArgs["myArg"], 8);  // copy macro data argument to buffer
		universalWrite(address, data);

		// command-#4: delay(4);
		__FE_COUT__ << "Sleeping for... " << 4 << " milliseconds " << __E__;
		usleep(4 * 1000 /* microseconds */);

		delete[] address;  // free the memory
		delete[] data;     // free the memory
	}

	for(auto& argOut : argsOut)
		__FE_COUT__ << argOut.first << ": " << argOut.second << __E__;

}  // end varTest2()

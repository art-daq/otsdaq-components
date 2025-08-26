#include <iostream>
#include <set>
#include "otsdaq-components/FEInterfaces/FEOtsUDPTemplateInterface.h"
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/MessageFacility/MessageFacility.h"

using namespace ots;

//==============================================================================
FEOtsUDPTemplateInterface::FEOtsUDPTemplateInterface(
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
	// registration of FEMacro 'test' generated, Jan-20-2020 09:41:49, by 'admin' using
	// MacroMaker.
	FEVInterface::registerFEMacroFunction(
	    "test",  // feMacroName
	    static_cast<FEVInterface::frontEndMacroFunction_t>(
	        &FEOtsUDPTemplateInterface::test),  // feMacroFunction
	    std::vector<std::string>{},             // namesOfInputArgs
	    std::vector<std::string>{"outArg1"},    // namesOfOutputArgs
	    1);                                     // requiredUserPermissions

	universalAddressSize_ = 8;
	universalDataSize_    = 8;

	// example self-call of FE-macros
	if(1)
	{
		// registration of FEMacro 'varTest2' generated, Oct-11-2018 02:28:57, by 'admin'
		// using MacroMaker.
		FEVInterface::registerFEMacroFunction(
		    "varTest2",  // feMacroName
		    static_cast<FEVInterface::frontEndMacroFunction_t>(
		        &FEOtsUDPTemplateInterface::varTest2),     // feMacroFunction
		    std::vector<std::string>{"myOtherArg"},        // namesOfInputArgs
		    std::vector<std::string>{"myArg", "outArg1"},  // namesOfOutputArgs
		    1);                                            // requiredUserPermissions

		// registration of FEMacro 'varTest' generated, Oct-11-2018 11:36:28, by 'admin'
		// using MacroMaker.
		FEVInterface::registerFEMacroFunction(
		    "varTest",  // feMacroName
		    static_cast<FEVInterface::frontEndMacroFunction_t>(
		        &FEOtsUDPTemplateInterface::varTest),      // feMacroFunction
		    std::vector<std::string>{"myOtherArg"},        // namesOfInputArgs
		    std::vector<std::string>{"myArg", "outArg1"},  // namesOfOutputArgs
		    1);                                            // requiredUserPermissions

		std::vector<frontEndMacroArg_t> argsIn;
		__SET_ARG_IN__("myOtherArg", (unsigned int)5);

		std::vector<frontEndMacroArg_t> argsOut;

		__FE_COUTV__(StringMacros::vectorToString(argsIn));

		runSelfFrontEndMacro("varTest2", argsIn, argsOut);

		__FE_COUTV__(StringMacros::vectorToString(argsOut));
		__FE_COUTV__(FEVInterface::getFEMacroArgument(argsOut, "outArg1"));

		{
			std::string    a = FEVInterface::getFEMacroArgument(argsOut, "myArg");
			double         b = getFEMacroArgumentValue<double>(argsOut, "outArg1");
			unsigned short c =
			    getFEMacroArgumentValue<unsigned short>(argsOut, "outArg1");
			__FE_COUTV__(a);
			__FE_COUTV__(b);
			__FE_COUTV__(c);
		}
	}  // end example of calling own FE-macros

	// example of using int, float, and string bitmaps
	if(1)
	{
		try
		{
			{
				ConfigurationTree bmpNode = getSelfNode().getNode("FloatBitmap");
				const TableViewColumnInfo::BitMapInfo& bitMapInfo =
				    bmpNode.getColumnInfo().getBitMapInfo();
				__FE_COUT__ << "Float BitMapInfo: rows=" << bitMapInfo.numOfRows_
				            << ", cols=" << bitMapInfo.numOfColumns_
				            << ", bitSize=" << bitMapInfo.cellBitSize_
				            << ", min=" << bitMapInfo.minValue_
				            << ", max=" << bitMapInfo.maxValue_
				            << ", step=" << bitMapInfo.stepValue_ << __E__;
				__FE_COUT__ << "  colors: min=" << bitMapInfo.minColor_
				            << ", mid=" << bitMapInfo.midColor_
				            << ", max=" << bitMapInfo.maxColor_ << __E__;
				__FE_COUT__ << "  flags: rowsAsc=" << bitMapInfo.rowsAscending_
				            << ", colsAsc=" << bitMapInfo.colsAscending_
				            << ", snakeRows=" << bitMapInfo.snakeRows_
				            << ", snakeCols=" << bitMapInfo.snakeCols_
				            << ", float=" << bitMapInfo.floatingPoint_ << __E__;

				{
					ConfigurationTree::BitMap<double> bmp =
					    bmpNode.getValueAsBitMap<double>();

					__FE_COUTV__(bmp.numberOfRows());
					__FE_COUTV__(bmp.numberOfColumns(0));
					__FE_COUTV__(bmp.get(0, 0));
					__FE_COUTV__(bmp.get(0, 1));
					__FE_COUTV__(bmp.get(0, 2));
					__FE_COUTV__(bmp.get(0, 3));
					__FE_COUTV__(bmp.get(0, 4));
					if(TTEST(1))
						bmp.print();
				}

				{
					ConfigurationTree::BitMap<uint8_t> bmp =
					    bmpNode.getValueAsBitMap<uint8_t>();

					__FE_COUTV__(bmp.numberOfRows());
					__FE_COUTV__(bmp.numberOfColumns(0));
					__FE_COUTV__((uint16_t)bmp.get(0, 0));
					__FE_COUTV__((uint16_t)bmp.get(0, 1));
					__FE_COUTV__((uint16_t)bmp.get(0, 2));
					__FE_COUTV__((uint16_t)bmp.get(0, 3));
					__FE_COUTV__((uint16_t)bmp.get(0, 4));
					if(TTEST(1))
						bmp.print();
				}
			}

			{
				ConfigurationTree bmpNode = getSelfNode().getNode("StringBitmap");
				TableViewColumnInfo::BitMapInfo bitMapInfo =
				    getSelfNode().getNode("StringBitmap").getColumnInfo().getBitMapInfo();
				__FE_COUT__ << "String BitMapInfo: rows=" << bitMapInfo.numOfRows_
				            << ", cols=" << bitMapInfo.numOfColumns_
				            << ", bitSize=" << bitMapInfo.cellBitSize_
				            << ", min=" << bitMapInfo.minValue_
				            << ", max=" << bitMapInfo.maxValue_
				            << ", step=" << bitMapInfo.stepValue_ << __E__;
				__FE_COUT__ << "  colors: min=" << bitMapInfo.minColor_
				            << ", mid=" << bitMapInfo.midColor_
				            << ", max=" << bitMapInfo.maxColor_ << __E__;
				__FE_COUT__ << "  flags: rowsAsc=" << bitMapInfo.rowsAscending_
				            << ", colsAsc=" << bitMapInfo.colsAscending_
				            << ", snakeRows=" << bitMapInfo.snakeRows_
				            << ", snakeCols=" << bitMapInfo.snakeCols_
				            << ", float=" << bitMapInfo.floatingPoint_ << __E__;
				{
					ConfigurationTree::BitMap<uint16_t> bmp =
					    bmpNode.getValueAsBitMap<uint16_t>();
					__FE_COUTV__(bmp.numberOfRows());
					__FE_COUTV__(bmp.numberOfColumns(0));
					__FE_COUTV__(bmp.get(0, 0));
					__FE_COUTV__(bmp.get(0, 1));
					__FE_COUTV__(bmp.get(0, 2));
					__FE_COUTV__(bmp.get(0, 3));
					__FE_COUTV__(bmp.get(0, 4));
					if(TTEST(1))
						bmp.print();
				}

				{
					ConfigurationTree::BitMap<std::string> bmp =
					    bmpNode.getValueAsBitMap<std::string>();
					__FE_COUTV__(bmp.numberOfRows());
					__FE_COUTV__(bmp.numberOfColumns(0));
					__FE_COUTV__(bmp.get(0, 0));
					__FE_COUTV__(bmp.get(0, 1));
					__FE_COUTV__(bmp.get(0, 2));
					__FE_COUTV__(bmp.get(0, 3));
					__FE_COUTV__(bmp.get(0, 4));
					if(TTEST(1))
						bmp.print();
				}

				{
					auto bmp = bmpNode.getValueAsBitMap<uint32_t>();
					__FE_COUTV__(bmp.numberOfRows());
					__FE_COUTV__(bmp.numberOfColumns(0));
					__FE_COUTV__(bmp.get(0, 0));
					__FE_COUTV__(bmp.get(0, 1));
					__FE_COUTV__(bmp.get(0, 2));
					__FE_COUTV__(bmp.get(0, 3));
					__FE_COUTV__(bmp.get(0, 4));
					if(TTEST(1))
						bmp.print();
				}
			}
		}
		catch(...)
		{
			__FE_COUT__ << "Ignoring exception caught while testing bitmaps." << __E__;
			try
			{
				throw;
			}
			catch(const std::runtime_error& e)
			{
				__FE_COUT__ << "Caught bitmap error: " << e.what() << __E__;
			}
			catch(const std::exception& e)
			{
				__FE_COUT__ << "Caught bitmap error: " << e.what() << __E__;
			}
			catch(...)
			{
				__FE_COUT__ << "Unknown error." << __E__;
			}
		}
	}  //end example of using int, float, and string bitmaps

}  // end constructor

//==============================================================================
FEOtsUDPTemplateInterface::~FEOtsUDPTemplateInterface(void) {}

//==============================================================================
void FEOtsUDPTemplateInterface::configure(void)
{
	// unsigned int i = VStateMachine::getIterationIndex();
	// unsigned int j = VStateMachine::getSubIterationIndex();
	//	if(i == 0 && j < 5)
	//	{
	//		VStateMachine::indicateSubIterationWork();
	//		sleep(3);
	//	}
	//	else if(i < 10)
	//	{
	//		VStateMachine::indicateIterationWork();
	//		sleep(1);
	//	}
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
	//  short>("ExampleInterface0");
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
}  // end configure()

//==============================================================================
/// void FEOtsUDPTemplateInterface::configureDetector(const DACStream& theDACStream)
//{
//	__FE_COUT__ << "\tconfigureDetector" << __E__;
//}

//==============================================================================
void FEOtsUDPTemplateInterface::halt(void)
{
	__FE_COUT__ << "\tHalt" << __E__;
	stop();
}

//==============================================================================
void FEOtsUDPTemplateInterface::pause(void)
{
	__FE_COUT__ << "\tPause" << __E__;
	stop();
}

//==============================================================================
void FEOtsUDPTemplateInterface::resume(void)
{
	__FE_COUT__ << "\tResume" << __E__;
	start("");
}

//==============================================================================
void FEOtsUDPTemplateInterface::start(std::string)  // runNumber)
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
void FEOtsUDPTemplateInterface::stop(void)
{
	__FE_COUT__ << "\tStop" << __E__;

	// Run Stop Sequence Commands

	FEVInterface::runSequenceOfCommands("LinkToStopSequence");

	std::string sendBuffer;
	OtsUDPFirmwareCore::stopBurst(sendBuffer);
	OtsUDPHardware::write(sendBuffer);
}

//==============================================================================
bool FEOtsUDPTemplateInterface::running(void)
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
			//						if(1 || getInterfaceUID() == "ExampleInterface1")
			//						{
			//							__FE_SS__ << "Soft Error Here" << __E__;
			//							throw __OTS_PAUSE_EXCEPTION__(ss.str());
			//						}
			//						else
			break;
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
void ots::FEOtsUDPTemplateInterface::universalRead(char* address, char* returnValue)
{
	__FE_COUT__ << "address size " << universalAddressSize_ << __E__;

	if(TTEST(0))
	{
		std::stringstream oss;
		oss << "Universal Read Address: 0x";
		for(unsigned int i = 0; i < universalAddressSize_; ++i)
			oss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
			    << (uint16_t)address[universalAddressSize_ - 1 - i];
		//sprintf(buff,"%2.2X", (unsigned char)address[universalAddressSize_ - 1 - i]);
		oss << __E__;
		__FE_COUT__ << oss.str();
	}

	std::string readBuffer, sendBuffer;
	OtsUDPFirmwareCore::readAdvanced(sendBuffer, address, 1 /*size*/);

	OtsUDPHardware::read(sendBuffer, readBuffer);  // data reply

	__FE_COUT__ << "Result SIZE: " << readBuffer.size() << __E__;
	std::memcpy(returnValue, readBuffer.substr(2).c_str(), universalDataSize_);

	if(TTEST(0))
	{
		std::stringstream oss;
		oss << "Universal Read Data: 0x";
		for(unsigned int i = 0; i < universalDataSize_; ++i)
			oss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
			    << (uint16_t)returnValue[universalDataSize_ - 1 - i];
		// printf("%2.2X", (unsigned char)returnValue[universalDataSize_ - 1 - i]);
		oss << __E__;
		__FE_COUT__ << oss.str();
	}

}  // end universalRead()

//==============================================================================
/// NOTE: buffer for address must be at least size universalAddressSize_
/// NOTE: buffer for writeValue must be at least size universalDataSize_
void ots::FEOtsUDPTemplateInterface::universalWrite(char* address, char* writeValue)
{
	if(TTEST(0))
	{
		__FE_COUT__ << "address size " << universalAddressSize_ << __E__;
		__FE_COUT__ << "data size " << universalDataSize_ << __E__;

		{
			std::stringstream oss;
			oss << "Universal Write Address: 0x";
			for(unsigned int i = 0; i < universalAddressSize_; ++i)
				oss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
				    << (uint16_t)address[universalAddressSize_ - 1 - i];
			// printf("%2.2X", (unsigned char)address[universalAddressSize_ - 1 - i]);
			oss << __E__;
			__FE_COUT__ << oss.str();
		}
		{
			std::stringstream oss;
			oss << "Universal Write Data: 0x";
			for(unsigned int i = 0; i < universalDataSize_; ++i)
				oss << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
				    << (uint16_t)writeValue[universalDataSize_ - 1 - i];
			// printf("%2.2X", (unsigned char)writeValue[universalDataSize_ - 1 - i]);
			oss << __E__;
			__FE_COUT__ << oss.str();
		}
	}

	std::string sendBuffer;
	OtsUDPFirmwareCore::writeAdvanced(sendBuffer, address, writeValue, 1 /*size*/);
	OtsUDPHardware::write(sendBuffer);  // data request
}  // end universalWrite()

//==============================================================================
/// varTest
///	FEMacro 'varTest' generated, Oct-11-2018 11:36:28, by 'admin' using MacroMaker.
///	Macro Notes: This is a great test!
void FEOtsUDPTemplateInterface::varTest(__ARGS__)
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

	__SET_ARG_OUT__(PLOTLY_PLOT,  //use built-in auto plotting using plotly
	                R"({
			"data" : [{
					"x": [1, 2, 3],
					"y": [4, 5, 6],
					"mode": "lines+markers",
					"type": "scatter",
					"name": "Line 1"
				},
				{
					"x": [1, 2, 3],
					"y": [8, 3, 4],
					"mode": "lines+markers",
					"type": "scatter",
					"name": "Line 2"
				}],
			"layout" : {
					"title" : { "text" : "Awesome plot"},
					"xaxis" : { "title" : {"text" : "time"}, "titlefont": { "size" : 10 }, "showticklabels" : true },
					"yaxis" : { "title" : {"text" : "amplitude"}, "titlefont": { "size" : 10 }, "zeroline" : true }
				}
		})");

	for(auto& argOut : argsOut)
		__FE_COUT__ << argOut.first << ": " << argOut.second << __E__;

}  // end varTest()

//==============================================================================
/// varTest2
///	FEMacro 'varTest2' generated, Oct-11-2018 02:28:57, by 'admin' using MacroMaker.
///	Macro Notes: [Modified 14:28 10/11/2018] This is a great test!
void FEOtsUDPTemplateInterface::varTest2(__ARGS__)
{
	__FE_COUT__ << "# of input args = " << argsIn.size() << __E__;
	__FE_COUT__ << "# of output args = " << argsOut.size() << __E__;
	for(auto& argIn : argsIn)
		__FE_COUT__ << argIn.first << ": " << argIn.second << __E__;

	__SET_ARG_OUT__("myArg", "hello2");  // update output argument result
	uint64_t macroData = __GET_ARG_IN__("myOtherArg", uint64_t);
	macroData *= 3;
	__SET_ARG_OUT__("outArg1", macroData);  // update output argument result

	for(auto& argOut : argsOut)
		__FE_COUT__ << argOut.first << ": " << argOut.second << __E__;

}  // end varTest2()

//========================================================================================================================
/// test
///	FEMacro 'test' generated, Jan-20-2020 09:41:49, by 'admin' using MacroMaker.
///	Macro Notes: Testing notes
void FEOtsUDPTemplateInterface::test(__ARGS__)
{
	__CFG_COUT__ << "# of input args = " << argsIn.size() << __E__;
	__CFG_COUT__ << "# of output args = " << argsOut.size() << __E__;
	for(auto& argIn : argsIn)
		__CFG_COUT__ << argIn.first << ": " << argIn.second << __E__;

	// macro commands section
	{
		char* address = new char[universalAddressSize_]{
		    0};  // create address buffer of interface size and init to all 0
		char* data = new char[universalDataSize_]{
		    0};                 // create data buffer of interface size and init to all 0
		uint64_t macroAddress;  // create macro address buffer (size 8 bytes)
		uint64_t macroData;     // create macro address buffer (size 8 bytes)
		std::map<std::string /*arg name*/, uint64_t /*arg val*/>
		    macroArgs;  // create map from arg name to 64-bit number

		// command-#0: Write(0x4 /*address*/,0x2 /*data*/);
		macroAddress = 0x4;
		memcpy(address, &macroAddress, 8);  // copy macro address to buffer
		macroData = 0x2;
		memcpy(data, &macroData, 8);  // copy macro data to buffer
		universalWrite(address, data);

		// command-#1: Read(0x4 /*address*/,data);
		macroAddress = 0x4;
		memcpy(address, &macroAddress, 8);  // copy macro address to buffer
		universalRead(address, data);
		memcpy(&macroArgs["outArg1"], data, 8);            // copy buffer to argument map
		__SET_ARG_OUT__("outArg1", macroArgs["outArg1"]);  // update output argument
		                                                   // result

		delete[] address;  // free the memory
		delete[] data;     // free the memory
	}

	for(auto& argOut : argsOut)
		__CFG_COUT__ << argOut.first << ": " << argOut.second << __E__;

}  // end test()

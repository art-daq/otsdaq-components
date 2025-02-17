#include "otsdaq-components/DAQHardware/OtsUDPFirmwareCore.h"

#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/MessageFacility/MessageFacility.h"

//#include "otsdaq/BitManipulator/BitManipulator.h"
//#include "otsdaq-components/DAQHardware/FirmwareBaseDefinitions.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
//#include
///"otsdaq-components/UserConfigurationDataFormats/FEOtsUDPFSSRInterfaceTable.h"
///
using namespace ots;

/////////////////////////////////=======================================
/// ADDRESS SPACE
///	http://otsdaq.fnal.gov/docs/oei_address_space.html
/// field size (bits)
#define UDP_CORE_BLOCK_ADDRESS ((uint64_t)(0x1) << 32)
#define ADDR_SELF_UPPER_IP (uint64_t)(0x0)  // 24
#define ADDR_SELF_LOWER_IP (uint64_t)(0x1)  // 8
#define ADDR_SELF_MAC (uint64_t)(0x2)       // 40

#define ADDR_CTRL_DEST_IP (uint64_t)(0x3)    // 32
#define ADDR_CTRL_DEST_MAC (uint64_t)(0x4)   // 48
#define ADDR_CTRL_DEST_PORT (uint64_t)(0x5)  // 16

#define ADDR_DATA_DEST_IP (uint64_t)(0x6)    // 32
#define ADDR_DATA_DEST_MAC (uint64_t)(0x7)   // 48
#define ADDR_DATA_DEST_PORT (uint64_t)(0x8)  // 16

#define ADDR_BURST_MODE (uint64_t)(0x9)         // 1
#define ADDR_CTRL_RESOLVE_MAC (uint64_t)(0xA)   // 1
#define ADDR_DATA_RESOLVE_MAC (uint64_t)(0xB)   // 1
#define ADDR_SELF_VERSION (uint64_t)(0x64)      // 16 (read only)
#define ADDR_SELF_RESET (uint64_t)(0xFFFFFFFF)  // 1  (write only)

/// end ADDRESS SPACE
/////////////////////////////////=======================================

const uint8_t OtsUDPFirmwareCore::FIFO_ADDRESS_CMD_TYPE = (1 << 3);

//==============================================================================
OtsUDPFirmwareCore::OtsUDPFirmwareCore(unsigned int version)
    : FrontEndFirmwareBase(version)
{
}

//==============================================================================
OtsUDPFirmwareCore::~OtsUDPFirmwareCore(void) {}

//==============================================================================
void OtsUDPFirmwareCore::init(void) {}

//==============================================================================
std::string OtsUDPFirmwareCore::write(char* address, char* data)
{
	std::string buffer;
	writeAdvanced(buffer, address, data);
	return buffer;
}

//==============================================================================
void OtsUDPFirmwareCore::write(std::string& buffer,
                               char*        address,
                               char*        data,
                               bool         clearBuffer)
{
	writeAdvanced(buffer, address, data, 1 /*size*/, 0 /*options*/, clearBuffer);
}

//==============================================================================
void OtsUDPFirmwareCore::write(std::string& buffer,
                               uint32_t     address,
                               uint32_t     data,
                               bool         clearBuffer)
{
	writeAdvanced(buffer, address, data, 0 /*options*/, clearBuffer);
}

//==============================================================================
void OtsUDPFirmwareCore::write(std::string& buffer,
                               uint64_t     address,
                               uint64_t     data,
                               bool         clearBuffer)
{
	writeAdvanced(buffer, address, data, 0 /*options*/, clearBuffer);
}

//==============================================================================
/// OtsUDPFirmwareCore::writeAdvanced
///	size is in units of quad-words (8-Bytes)
void OtsUDPFirmwareCore::OtsUDPFirmwareCore::writeAdvanced(std::string& buffer,
                                                           const char*  address,
                                                           const char*  data,
                                                           uint8_t      size,
                                                           uint8_t commandTypeOptions,
                                                           bool    clearBuffer)
{
	if(size > 182)
	{
		__SS__ << "Invalid OtsUDPFirmwareCore::writeAdvanced size: " << size << std::endl;
		__COUT_ERR__ << "\n" << ss.str();
		__SS_THROW__;
	}

	if(buffer.size() + 2 + 8 + 8 * size > 2 + 8 + 8 * 182)
	{
		__SS__ << "Aborting. Buffer overflow attempt: " << buffer.size() << " + "
		       << 2 + 8 + 8 * size << " > " << 2 + 8 + 8 * 182 << std::endl;
		__COUT_ERR__ << "\n" << ss.str();
		__SS_THROW__;
	}

	if(clearBuffer)
		buffer.resize(0);

	unsigned int begin = buffer.length();
	buffer.resize(buffer.length() + 2 + 8 + 8 * size, '\0');

	buffer[begin + 0] = 1 | commandTypeOptions;  // OtsUDPFirmwareCore::writeAdvanced
	buffer[begin + 1] = size;                    //# of 64 bits words

	std::copy_n(address, sizeof(uint64_t), &buffer[begin + 2]);
	for(uint8_t i = 0; i < size; ++i)
		std::copy_n(
		    (char*)&data[8 * i], sizeof(uint64_t), &buffer[begin + 2 + 8 + 8 * i]);

	///////
	/*
	__SS__ << "\tAdded OtsUDPFirmwareCore::writeAdvanced to Buffer:-";
	for(uint32_t i=begin; i<buffer.size(); i++)
	{
	    if(i==begin+2) ss << ":::";
	    else if(i==begin+10) ss << ":::";
	    ss << std::setfill('0') << std::setw(2) << std::hex << (((int16_t) buffer[i])
	&0xFF) << "-" << std::dec;
	}
	ss << " added " << (buffer.size() - begin) << " to " << buffer.size();
	ss << std::endl;
	__COUT__ << "\n" << ss.str();
	*/
}

//==============================================================================
/// OtsUDPFirmwareCore::writeAdvanced
/// 	Note: size is required, to remove ambiguity when calling
/// OtsUDPFirmwareCore::writeAdvanced with data=0
void OtsUDPFirmwareCore::OtsUDPFirmwareCore::writeAdvanced(std::string&    buffer,
                                                           const uint64_t& address,
                                                           const char*     data,
                                                           uint8_t         size,
                                                           uint8_t commandTypeOptions,
                                                           bool    clearBuffer)
{
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer, (char*)&address, data, size, commandTypeOptions, clearBuffer);
}

//==============================================================================
void OtsUDPFirmwareCore::OtsUDPFirmwareCore::writeAdvanced(std::string&    buffer,
                                                           const uint64_t& address,
                                                           const uint64_t& data,
                                                           uint8_t commandTypeOptions,
                                                           bool    clearBuffer)
{
	OtsUDPFirmwareCore::writeAdvanced(buffer,
	                                  (char*)&address,
	                                  (char*)&data,
	                                  1 /*size*/,
	                                  commandTypeOptions,
	                                  clearBuffer);
}

//==============================================================================
void OtsUDPFirmwareCore::OtsUDPFirmwareCore::writeAdvanced(
    std::string&                 buffer,
    const uint64_t&              address,
    const std::vector<uint64_t>& data,
    uint8_t                      commandTypeOptions,
    bool                         clearBuffer)
{
	OtsUDPFirmwareCore::writeAdvanced(buffer,
	                                  (char*)&address,
	                                  (char*)&data[0],
	                                  data.size() /*size*/,
	                                  commandTypeOptions,
	                                  clearBuffer);
}

//==============================================================================
std::string OtsUDPFirmwareCore::read(char* address)
{
	__COUT__ << "OTS pointer based read!! " << std::endl;
	std::string buffer;
	readAdvanced(buffer, address);
	return buffer;
}

//==============================================================================
void OtsUDPFirmwareCore::read(std::string& buffer, char* address, bool clearBuffer)
{
	readAdvanced(buffer, address, 1 /*size*/, 0 /*options*/, clearBuffer);
}

//==============================================================================
void OtsUDPFirmwareCore::read(std::string& buffer, uint64_t address, bool clearBuffer)
{
	readAdvanced(buffer, address, 1 /*size*/, 0 /*options*/, clearBuffer);
}

//==============================================================================
void OtsUDPFirmwareCore::read(std::string& buffer, uint32_t address, bool clearBuffer)
{
	readAdvanced(buffer, address, 1 /*size*/, 0 /*options*/, clearBuffer);
}

//==============================================================================
void OtsUDPFirmwareCore::OtsUDPFirmwareCore::readAdvanced(std::string& buffer,
                                                          char*        address,
                                                          uint8_t      size,
                                                          uint8_t      commandTypeOptions,
                                                          bool         clearBuffer)
{
	if(size > 182)
	{
		__SS__ << "Invalid OtsUDPFirmwareCore::readAdvanced size: " << size << std::endl;
		__COUT_ERR__ << "\n" << ss.str();
		__SS_THROW__;
	}

	if(buffer.size() + 2 + 8 > 2 + 8 + 8 * 182)
	{
		__SS__ << "Aborting. Buffer overflow attempt: " << buffer.size() << " + " << 2 + 8
		       << " > " << 2 + 8 + 8 * 182 << std::endl;
		__COUT_ERR__ << "\n" << ss.str();
		__SS_THROW__;
	}

	if(clearBuffer)
		buffer.resize(0);

	unsigned int begin = buffer.length();
	buffer.resize(buffer.length() + 2 + 8, '\0');

	buffer[begin + 0] =
	    (uint8_t)0 | commandTypeOptions;  // OtsUDPFirmwareCore::readAdvanced
	buffer[begin + 1] = size;             //# of 64 bits words

	// NOTE -- be wary of copy_n RAR -- it failed for me with uint32_t size
	std::copy_n(address, sizeof(uint64_t), &buffer[begin + 2]);

	///////
	//__SS__ << "\tAdded OtsUDPFirmwareCore::readAdvanced to Buffer:-";
	// for(uint32_t i=begin; i<buffer.size(); i++)
	//{
	//	if(i==begin+2) ss << ":::";
	//	else if(i==begin+10) ss << ":::";
	//	ss << std::setfill('0') << std::setw(2) << std::hex << (((int16_t) buffer[i])
	//&0xFF) << "-" << std::dec;
	//}
	// ss << std::endl;
	//__COUT__ << "\n" << ss.str();
}

//==============================================================================
void OtsUDPFirmwareCore::OtsUDPFirmwareCore::readAdvanced(std::string&    buffer,
                                                          const uint64_t& address,
                                                          uint8_t         size,
                                                          uint8_t commandTypeOptions,
                                                          bool    clearBuffer)
{
	OtsUDPFirmwareCore::readAdvanced(
	    buffer, (char*)&address, size, commandTypeOptions, clearBuffer);
}

//==============================================================================
void OtsUDPFirmwareCore::setDataDestination(std::string&       buffer,
                                            const std::string& ipAddress,
                                            const uint16_t     port,
                                            bool               clearBuffer)
{
	// put firmware in dynamic mac resolution mode for burst mode
	//__COUT__ << "Dynamic mac address: " << std::hex <<
	//		uint64_t(UDP_CORE_BLOCK_ADDRESS | ADDR_DATA_RESOLVE_MAC) << std::endl;
	OtsUDPFirmwareCore::write(buffer,
	                          UDP_CORE_BLOCK_ADDRESS | ADDR_DATA_RESOLVE_MAC /* addr */,
	                          1 /* data */,
	                          clearBuffer);

	// extract ip address from string
	//__COUT__ << "IP Address = " << ipAddress << std::endl;
	uint64_t ip = 0;
	int      ipArr[4];
	sscanf(ipAddress.c_str(), "%d.%d.%d.%d", &ipArr[3], &ipArr[2], &ipArr[1], &ipArr[0]);
	for(int i = 0; i < 4; ++i)
		(((char*)&ip)[i]) += ipArr[i] & 0x0FF;

	writeDataDestinationIP(buffer, ip, false /*clearBuffer*/);
	writeDataDestinationPort(buffer, port, false /*clearBuffer*/);
}

//==============================================================================
void OtsUDPFirmwareCore::setControlDestination(std::string&       buffer,
                                               const std::string& ipAddress,
                                               const uint16_t     port)
{
	// put firmware in dynamic mac resolution mode for burst mode
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_CTRL_RESOLVE_MAC /* addr */, 1 /* data */);

	// extract ip address from string
	__COUT__ << "ipAddress = " << ipAddress << std::endl;
	uint64_t ip = 0;
	int      ipArr[4];
	sscanf(ipAddress.c_str(), "%d.%d.%d.%d", &ipArr[3], &ipArr[2], &ipArr[1], &ipArr[0]);
	for(int i = 0; i < 4; ++i)
		(((char*)&ip)[i]) += ipArr[i] & 0x0FF;

	writeControlDestinationIP(buffer, ip);
	writeControlDestinationPort(buffer, port);
}

//==============================================================================
/// return data lower 32 bits
uint32_t OtsUDPFirmwareCore::createRegisterFromValue(std::string& /*readBuffer*/,
                                                     std::string& receivedValue)
{
	uint32_t retVal = *((uint32_t*)(&receivedValue[2]));

	// for(unsigned int i=0;i<receivedValue.size();++i)
	//	__COUT__ << std::hex << (int)receivedValue[i] << std::dec << std::endl;
	//__COUT__ << "Register value: 0x" << std::hex << retVal << std::dec << std::endl;
	return retVal;
}

//==============================================================================
void OtsUDPFirmwareCore::writeDataDestinationIP(std::string&   buffer,
                                                const uint64_t value,
                                                bool           clearBuffer)
{
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer,
	    UDP_CORE_BLOCK_ADDRESS | ADDR_DATA_DEST_IP /* addr */,
	    value /* data */,
	    0 /*options*/,
	    clearBuffer);
}

//==============================================================================
void OtsUDPFirmwareCore::writeDataDestinationMAC(std::string&   buffer,
                                                 const uint64_t value,
                                                 bool           clearBuffer)
{
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer,
	    UDP_CORE_BLOCK_ADDRESS | ADDR_DATA_DEST_MAC /* addr */,
	    value /* data */,
	    0 /*options*/,
	    clearBuffer);
}

//==============================================================================
void OtsUDPFirmwareCore::writeDataDestinationPort(std::string&   buffer,
                                                  const uint64_t value,
                                                  bool           clearBuffer)
{
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer,
	    UDP_CORE_BLOCK_ADDRESS | ADDR_DATA_DEST_PORT /* addr */,
	    value /* data */,
	    0 /*options*/,
	    clearBuffer);
}

//==============================================================================
void OtsUDPFirmwareCore::readDataDestinationIP(std::string& buffer)
{
	OtsUDPFirmwareCore::readAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_DATA_DEST_IP /* addr */);
}

//==============================================================================
void OtsUDPFirmwareCore::readDataDestinationMAC(std::string& buffer)
{
	OtsUDPFirmwareCore::readAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_DATA_DEST_MAC /* addr */);
}

//==============================================================================
void OtsUDPFirmwareCore::readDataDestinationPort(std::string& buffer)
{
	OtsUDPFirmwareCore::readAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_DATA_DEST_PORT /* addr */);
}

//==============================================================================
void OtsUDPFirmwareCore::writeControlDestinationIP(std::string&   buffer,
                                                   const uint64_t value)
{
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_CTRL_DEST_IP /* addr */, value /* data */);
}

//==============================================================================
void OtsUDPFirmwareCore::writeControlDestinationMAC(std::string&   buffer,
                                                    const uint64_t value)
{
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_CTRL_DEST_MAC /* addr */, value /* data */);
}

//==============================================================================
void OtsUDPFirmwareCore::writeControlDestinationPort(std::string&   buffer,
                                                     const uint64_t value)
{
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer,
	    UDP_CORE_BLOCK_ADDRESS | ADDR_CTRL_DEST_PORT /* addr */,
	    value /* data */);
}

//==============================================================================
void OtsUDPFirmwareCore::readControlDestinationIP(std::string& buffer)
{
	OtsUDPFirmwareCore::readAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_CTRL_DEST_IP /* addr */);
}

//==============================================================================
void OtsUDPFirmwareCore::readControlDestinationMAC(std::string& buffer)
{
	OtsUDPFirmwareCore::readAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_CTRL_DEST_MAC /* addr */);
}

//==============================================================================
void OtsUDPFirmwareCore::readControlDestinationPort(std::string& buffer)
{
	OtsUDPFirmwareCore::readAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_CTRL_DEST_PORT /* addr */);
}

//==============================================================================
void OtsUDPFirmwareCore::readUDPFirmwareVersion(std::string& buffer)
{
	OtsUDPFirmwareCore::readAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_SELF_VERSION /* addr */);
}

//==============================================================================
void OtsUDPFirmwareCore::softEthernetReset(std::string& buffer)
{
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_SELF_RESET /* addr */, 0x3 /* data */);
}

//==============================================================================
void OtsUDPFirmwareCore::hardEthernetReset(std::string& buffer)
{
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_SELF_RESET /* addr */, 0x1 /* data */);
}

//==============================================================================
void OtsUDPFirmwareCore::clearEthernetReset(std::string& buffer)
{
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_SELF_RESET /* addr */, 0 /* data */);
}

//==============================================================================
void OtsUDPFirmwareCore::startBurst(std::string& buffer)
{
	__COUT__ << std::endl;
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_BURST_MODE /* addr */, 1 /* data */);
}

//==============================================================================
void OtsUDPFirmwareCore::stopBurst(std::string& buffer)
{
	__COUT__ << std::endl;
	OtsUDPFirmwareCore::writeAdvanced(
	    buffer, UDP_CORE_BLOCK_ADDRESS | ADDR_BURST_MODE /* addr */, 0 /* data */);
}

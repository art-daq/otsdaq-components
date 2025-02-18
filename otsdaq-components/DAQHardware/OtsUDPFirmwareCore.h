#ifndef _ots_OtsUDPFirmwareCore_h_
#define _ots_OtsUDPFirmwareCore_h_

//#include "otsdaq-fermilabtestbeam/DAQHardware/FirmwareSequence.h"
#include "otsdaq-components/DAQHardware/FrontEndFirmwareBase.h"

#include <string>
#include <vector>

namespace ots
{
class OtsUDPFirmwareCore : public FrontEndFirmwareBase
{
	//===============================================
	/// OtsUDPFirmwareCore
	///
	///	The intent of this class is to be the core UDP firmware functionality.
	///	This should handle everything except the user block 0x0 of the address space.
	///
	///
	///		Note: Currently there are these other blocks of the address space:
	///			block 0x1 -- UDP core
	///			block 0x2 -- Programming over Ethernet core
	///
  public:
	OtsUDPFirmwareCore(unsigned int version);
	virtual ~OtsUDPFirmwareCore(void);
	virtual void init(void);

	/////////////////////////////////////
	/// implementation of base class virtual functionality
	///
	/// std::string read	  (char* address);
	virtual std::string write(char* address, char* data);
	virtual void        write(std::string& buffer,
	                          char*        address,
	                          char*        data,
	                          bool         clearBuffer = true);
	virtual void        write(std::string& buffer,
	                          uint32_t     address,
	                          uint32_t     data,
	                          bool         clearBuffer = true);
	virtual void        write(std::string& buffer,
	                          uint64_t     address,
	                          uint64_t     data,
	                          bool         clearBuffer = true);
	virtual void        waitClear(std::string& /*buffer*/,
	                              uint32_t /*address*/,
	                              uint32_t /*data*/,
	                              uint32_t /*timeout*/ = 255,
	                              bool /*clearBuffer*/ = true)
	{
	}

	virtual std::string read(char* address);
	virtual void        read(std::string& buffer, char* address, bool clearBuffer = true);
	virtual void read(std::string& buffer, uint64_t address, bool clearBuffer = true);
	virtual void read(std::string& buffer, uint32_t address, bool clearBuffer = true);

	virtual void setDataDestination(std::string&       buffer,
	                                const std::string& ip,
	                                const uint16_t     port,
	                                bool               clearBuffer = true);

	virtual uint32_t createRegisterFromValue(std::string& readBuffer,
	                                         std::string& receivedValue);
	/////////////////////////////////////
	/// low level functionality
	void writeAdvanced(std::string& buffer,
	                   const char*  address,
	                   const char*  data,
	                   uint8_t      size               = 1,
	                   uint8_t      commandTypeOptions = 0,
	                   bool         clearBuffer        = true);
	void writeAdvanced(std::string&    buffer,
	                   const uint64_t& address,
	                   const char*     data,
	                   uint8_t         size,
	                   uint8_t         commandTypeOptions = 0,
	                   bool clearBuffer = true);  ///< size is required, to remove ambiguity
	                                              ///< when calling write with data=0
	void writeAdvanced(std::string&    buffer,
	                   const uint64_t& address,
	                   const uint64_t& data,
	                   uint8_t         commandTypeOptions = 0,
	                   bool            clearBuffer        = true);
	void writeAdvanced(std::string&                 buffer,
	                   const uint64_t&              address,
	                   const std::vector<uint64_t>& data,
	                   uint8_t                      commandTypeOptions = 0,
	                   bool                         clearBuffer        = true);
	void readAdvanced(std::string& buffer,
	                  char*        address,
	                  uint8_t      size               = 1,
	                  uint8_t      commandTypeOptions = 0,
	                  bool         clearBuffer        = true);
	void readAdvanced(std::string&    buffer,
	                  const uint64_t& address,
	                  uint8_t         size               = 1,
	                  uint8_t         commandTypeOptions = 0,
	                  bool            clearBuffer        = true);

	/////////////////////////////////////
	/// next level functionality
	///
	/// data destination read/write
	void writeDataDestinationIP(std::string&   buffer,
	                            const uint64_t value,
	                            bool           clearBuffer = true);
	void writeDataDestinationMAC(std::string&   buffer,
	                             const uint64_t value,
	                             bool           clearBuffer = true);
	void writeDataDestinationPort(std::string&   buffer,
	                              const uint64_t value,
	                              bool           clearBuffer = true);

	void readDataDestinationIP(std::string& buffer);
	void readDataDestinationMAC(std::string& buffer);
	void readDataDestinationPort(std::string& buffer);

	void setDataDestinationAsSelf(std::string& buffer)
	{
		buffer.resize(0);
		buffer += (char)2; /* special one-byte packet, sets data destination as sender */
	}

	/// control destination read/write
	virtual void setControlDestination(std::string&       buffer,
	                                   const std::string& ip,
	                                   const uint16_t     port);

	void writeControlDestinationIP(std::string& buffer, const uint64_t value);
	void writeControlDestinationMAC(std::string& buffer, const uint64_t value);
	void writeControlDestinationPort(std::string& buffer, const uint64_t value);

	void readControlDestinationIP(std::string& buffer);
	void readControlDestinationMAC(std::string& buffer);
	void readControlDestinationPort(std::string& buffer);

	void setControlDestinationAsSelf(std::string& buffer)
	{
		buffer.resize(0);
		buffer +=
		    (char)1; /* special one-byte packet, sets control destination as sender */
	}

	/// other
	void UDPFirmwareSpecialPing(std::string& buffer)
	{
		buffer.resize(0);
		buffer += (char)0; /* special one-byte packet, returns OEI version (16-bits) */
	}
	void UDPFirmwareSpecialNoOp(std::string& buffer)
	{
		buffer.resize(0);
		buffer += (char)3; /* special one-byte packet, is ignored at OIE - could be used
		                      to keep connection alive */
	}

	void readUDPFirmwareVersion(std::string& buffer);
	void softEthernetReset(std::string& buffer);
	void hardEthernetReset(std::string& buffer);
	void clearEthernetReset(std::string& buffer);

	void startBurst(std::string& buffer);
	void stopBurst(std::string& buffer);

	/// command type options that can be OR'd
	static const uint8_t FIFO_ADDRESS_CMD_TYPE;
};
}  // namespace ots

#endif

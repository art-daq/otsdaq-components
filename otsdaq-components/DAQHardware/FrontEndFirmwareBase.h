#ifndef _ots_FrontEndFirmwareBase_h_
#define _ots_FrontEndFirmwareBase_h_

#include <iostream>
#include <string>
#include <vector>
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/MessageFacility/MessageFacility.h"

namespace ots
{
class FrontEndFirmwareBase
{
  public:
	/// factory method for choosing network protocol
	///
	/// FrontEndFirmwareBase          	(void){;}
	FrontEndFirmwareBase(unsigned int version = -1) : version_(version) { ; }
	virtual ~FrontEndFirmwareBase(void) { ; }
	virtual void init(void) { ; }

	unsigned int getVersion() { return version_; }

	/// These should never be called directly if used correctly, but
	/// not all classes will implement every function (so no pure virtuals). Should be
	/// obvious  that the wrong thing is happening if these are called because exceptions
	/// are thrown!
	///
	virtual std::string read(char* /*address*/)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return "";
	};
	virtual void read(std::string& /*buffer*/,
	                  char* /*address*/,
	                  bool /*clearBuffer*/ = true)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return;
	};
	virtual void read(std::string& /*buffer*/,
	                  uint64_t /*address*/,
	                  bool /*clearBuffer*/ = true)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return;
	};
	virtual void read(std::string& /*buffer*/,
	                  uint32_t /*address*/,
	                  bool /*clearBuffer*/ = true)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return;
	};

	virtual std::string write(char* /*address*/, char* /*data*/)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return "";
	};
	virtual void write(std::string& /*buffer*/,
	                   char* /*address*/,
	                   char* /*data*/,
	                   bool /*clearBuffer*/ = true)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return;
	};

	virtual void setDataDestination(std::string& /*buffer*/,
	                                const std::string& /*ip*/,
	                                const uint16_t /*port*/,
	                                bool /*clearBuffer*/ = true)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return;
	};
	virtual void write(std::string& /*buffer*/,
	                   uint64_t /*address*/,
	                   uint64_t /*data*/,
	                   bool /*clearBuffer*/ = true)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return;
	};
	virtual void write(std::string& /*buffer*/,
	                   uint32_t /*address*/,
	                   uint32_t /*data*/,
	                   bool /*clearBuffer*/ = true)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return;
	};
	virtual void waitSet(std::string& /*buffer*/,
	                     uint32_t /*address*/,
	                     uint32_t /*data*/,
	                     uint32_t /*timeout*/ = 255,
	                     bool /*clearBuffer*/ = true)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return;
	};
	virtual void waitClear(std::string& /*buffer*/,
	                       uint32_t /*address*/,
	                       uint32_t /*data*/,
	                       uint32_t /*timeout*/ = 255,
	                       bool /*clearBuffer*/ = true)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return;
	};
	virtual unsigned int getNumberOfBufferedCommands(std::string& /*buffer*/)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return 0;
	};
	/// virtual std::string  compareSendAndReceive       (const std::string& sentBuffer,
	/// std::string& acknowledgment)						{__SS__; __THROW__(ss.str() +
	/// "Illegal call to undefined base class member function"); return "";};
	virtual uint32_t createRegisterFromValue(std::string& /*readBuffer*/,
	                                         std::string& /*receivedValue*/)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return 0;
	};
	/// virtual uint64_t     createRegisterFromValue     (std::string& readBuffer,
	/// std::string& receivedValue)		{__SS__; __THROW__(ss.str() + "Illegal call to
	/// undefined base class member function"); return 0;};
	///
	/// purdue firmware
	///    virtual std::string  setDataDestination          (std::string ip, unsigned int
	///    port); virtual unsigned int waitSet                     (std::string& buffer,
	///    unsigned int address, unsigned int data, unsigned int timeout = 255); virtual
	///    unsigned int waitClear                   (std::string& buffer, unsigned int
	///    address, unsigned int data, unsigned int timeout = 255); virtual unsigned int
	///    getNumberOfBufferedCommands (std::string& buffer); virtual std::string
	///    compareSendAndReceive       (const std::string& sentBuffer, std::string&
	///    acknowledgment); virtual uint32_t     createRegisterFromValue     (std::string&
	///    readBuffer, std::string& receivedValue);
	///
	/// FSSROtsFirmware
	///    virtual void makeDACSequence 				(FirmwareSequence<uint64_t>& sequence,
	///    unsigned int channel, const ROCStream& rocStream); virtual void makeMaskSequence
	///    (FirmwareSequence<uint64_t>& sequence, unsigned int channel, const ROCStream&
	///    rocStream);
	///    virtual void makeDACSequence 				(FirmwareSequence<uint32_t>& sequence,
	///    unsigned int channel, const ROCStream& rocStream); virtual void makeDACBuffer
	///    (std::string& buffer, unsigned int channel, const ROCStream& rocStream); virtual
	///    void makeDACBuffer   				(std::vector<std::string>& buffer, unsigned
	///    int channel, const ROCStream& rocStream); virtual void makeMaskSequence
	///    (FirmwareSequence<uint32_t>& sequence, unsigned int channel, const ROCStream&
	///    rocStream);
	///    virtual void makeMaskBuffer  				(std::string& buffer, unsigned int
	///    channel, const ROCStream& rocStream);
	///
	/// for generic interface
	/*these functions have to be in FirmwareBase because the FEWOtsGenericInterface wants
	 *to access them through a pointer to FirmwareBase. Might want to change how that
	 *works.
	 */
	///    virtual std::string readCSRRegister(void)
	///    {
	///    	__SS__; __THROW__(ss.str() + "Illegal call to undefined base class member
	///    function"); 	std::cout << __COUT_HDR_FL__ << "Something bad happened!" <<
	///    std::endl; 	return "";
	///    };
	///    virtual void makeDACBuffer(std::vector<std::string>& buffer, unsigned int
	///    channel, const ROCStream& rocStream){__SS__; __THROW__(ss.str() + "Illegal call
	///    to undefined base class member function");  return;}; virtual void
	///    makeMaskBuffer(std::string& buffer, unsigned int channel, const ROCStream&
	///    rocStream){__SS__; __THROW__(ss.str() + "Illegal call to undefined base class
	///    member function");  return;};
	///
	/// static FrontEndFirmwareBase* getInstance(std::string choice, unsigned int version);
	///
	const unsigned int version_;
};
}  // namespace ots

#endif

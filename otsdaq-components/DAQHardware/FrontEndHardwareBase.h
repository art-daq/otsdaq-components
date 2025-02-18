/*
 * FrontEndHardwareBase.h
 *
 *  Created on: Aug 26, 2015
 *      Author: uplegger
 */

#ifndef _ots_FrontEndHardwareBase_h_
#define _ots_FrontEndHardwareBase_h_

#include "otsdaq/Macros/CoutMacros.h"

#include <stdexcept> /*for std::runtime_error*/
#include <string>
#include <vector>

namespace ots
{
class FrontEndHardwareBase
{
  public:
	FrontEndHardwareBase(unsigned int version = -1) : version_(version) { ; }
	virtual ~FrontEndHardwareBase() { ; }

	/// These should never be called directly if used correctly, but
	/// not all classes will implement every function (so no pure virtuals). Should be
	/// obvious  that the wrong thing is happening if these are called because exceptions
	/// are thrown!
	///
	virtual void write(const std::string& /*sendBuffer*/)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
	}
	virtual void write(const std::vector<std::string>& /*sendBuffers*/)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
	}
	virtual void writeAndAcknowledge(const std::string& /*sendBuffer*/,
	                                 int /*timeoutSeconds*/ = -1)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
	}
	virtual void writeAndAcknowledge(const std::vector<std::string>& /*sendBuffers*/,
	                                 int /*timeoutSeconds*/ = -1)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
	}
	virtual void read(const std::string& /*sendBuffer*/,
	                  std::string& /*receiveBuffer*/,
	                  int /*timeoutSeconds*/ = -1)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
	}
	virtual void read(const std::vector<std::string>& /*sendBuffers*/,
	                  std::vector<std::string>& /*receiveBuffers*/,
	                  int /*timeoutSeconds*/ = -1)
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
	}

	/// return count of 'things' flushed
	virtual int flushRead()
	{
		__SS__;
		__THROW__(ss.str() + "Illegal call to undefined base class member function");
		return 0;
	}

  protected:
	unsigned int version_;
};

}  // namespace ots

#endif  // _ots_FrontEndHardwareBase_h_

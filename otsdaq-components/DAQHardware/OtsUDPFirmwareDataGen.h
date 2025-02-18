#ifndef _ots_OtsUDPFirmwareDataGen_h_
#define _ots_OtsUDPFirmwareDataGen_h_

#include <string>

#include "otsdaq-components/DAQHardware/OtsUDPFirmwareCore.h"

namespace ots
{
class OtsUDPFirmwareDataGen : public OtsUDPFirmwareCore
{
	//===============================================
	/// OtsUDPFirmwareDataGen
	///
	///	The intent of this class is to be the "Data Gen" project firmware user
	/// functionality. 	This should handle everything in the user block 0x0 of the address
	/// space.
	///
	///
  public:
	OtsUDPFirmwareDataGen(unsigned int version);
	virtual ~OtsUDPFirmwareDataGen(void);
	virtual void init(void);

  protected:
	void setNumberOfBurstWords(std::string& buffer, uint64_t numberOfWords);
	void setBurstWordsRate(std::string& buffer, uint64_t interval);
};
}  // namespace ots

#endif

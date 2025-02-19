#ifndef _ots_OtsUDPHardware_h_
#define _ots_OtsUDPHardware_h_

#include "otsdaq-components/DAQHardware/FrontEndHardwareBase.h"
#include "otsdaq/NetworkUtilities/TransceiverSocket.h"

namespace ots
{
class OtsUDPHardware : public TransceiverSocket, public FrontEndHardwareBase
{
  public:
	OtsUDPHardware(std::string  hostIPAddress,
	               unsigned int hostPort,
	               std::string  boardIPAddress,
	               unsigned int boardPort,
	               unsigned int version = -1,
	               bool         verbose = false);
	virtual ~OtsUDPHardware(void);

	virtual void write(const std::string& sendBuffer);
	virtual void write(const std::vector<std::string>& sendBuffers);
	virtual void writeAndAcknowledge(const std::string& sendBuffer,
	                                 int                timeoutSeconds = -1);
	virtual void writeAndAcknowledge(const std::vector<std::string>& sendBuffers,
	                                 int                             timeoutSeconds = -1);
	virtual void read(const std::string& sendBuffer,
	                  std::string&       receiveBuffer,
	                  int                timeoutSeconds = -1);
	virtual void read(const std::vector<std::string>& sendBuffers,
	                  std::vector<std::string>&       receiveBuffers,
	                  int                             timeoutSeconds = -1);

	virtual int flushRead() { return clearReadSocket(); }

	void read(const std::string& sendBuffer,
	          uint64_t&          receiveQuadWord,
	          int                timeoutSeconds = -1);
	void read(const std::string&     sendBuffer,
	          std::vector<uint64_t>& receiveQuadWords,
	          int                    timeoutSeconds = -1);
	void read(const std::vector<std::string>&      sendBuffers,
	          std::vector<std::vector<uint64_t> >& receiveQuadWordsVector,
	          int                                  timeoutSeconds = -1);
	int  clearReadSocket();
	///
	///    virtual int write(uint64_t address, const std::string& value){return 0;}
	///    virtual int read (uint64_t address, std::string& value){return 0;}
	///
  protected:
	OtsUDPHardware(std::string  boardIPAddress,
	               unsigned int boardPort,
	               unsigned int version = -1,
	               bool         verbose = false);
	Socket      OtsUDPBoard_;
	std::string acknowledgment_;
	bool        verbose_;
};
}  // namespace ots

#endif

#include "otsdaq-components/DAQHardware/OtsUDPHardware.h"
#include <iomanip>
#include <iostream>
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/MessageFacility/MessageFacility.h"

using namespace ots;

//==============================================================================
/// This one is often (e.g. FENIMPlusInterface) called by FEs inheriting OtsUDPHardware
OtsUDPHardware::OtsUDPHardware(std::string  boardIPAddress,
                               unsigned int boardPort,
                               unsigned int version,
                               bool         verbose)
    :  // Socket				() default constructor
    FrontEndHardwareBase(version)
    , OtsUDPBoard_(boardIPAddress, boardPort)
    , verbose_(verbose)
{
	Socket::initialize();

	//	char msg[100];
	//	sprintf(msg,"_%d",getPort());
	//	std::string fn = "/tmp/new_udp_chk" + std::string(msg) + ".dat";
	//	FILE *fp = fopen(fn.c_str(),"w");
	//	if(fp) fclose(fp);
	//	__COUT__ << fn << std::endl;
}

//==============================================================================
OtsUDPHardware::OtsUDPHardware(std::string  hostIPAddress,
                               unsigned int hostPort,
                               std::string  OtsUDPHardwareIPAddress,
                               unsigned int OtsUDPHardwarePort,
                               unsigned int version,
                               bool         verbose)
    : Socket(hostIPAddress, hostPort)
    , FrontEndHardwareBase(version)
    , OtsUDPBoard_(OtsUDPHardwareIPAddress, OtsUDPHardwarePort)
    , verbose_(verbose)
{
	Socket::initialize();

	__COUT__ << "Socket for hardware is initialized at IP:Port "
	         << OtsUDPBoard_.getIPAddress() << ":" << OtsUDPBoard_.getPort() << __E__;

	//	char msg[100];
	//	sprintf(msg,"_%d",getPort());
	//	std::string fn = "/tmp/new_udp_chk" + std::string(msg) + ".dat";
	//	FILE *fp = fopen(fn.c_str(),"w");
	//	if(fp) fclose(fp);
	//	__COUT__ << fn << std::endl;
}

//==============================================================================
OtsUDPHardware::~OtsUDPHardware(void) {}

//==============================================================================
void OtsUDPHardware::write(const std::string& sendBuffer)
try
{
	//	char msg[100];
	//	sprintf(msg,"_%d",getPort());
	//	std::string fn = "/tmp/new_udp_chk" + std::string(msg) + ".dat";
	//	FILE *fp = fopen(fn.c_str(),"a");
	//	__COUT__ << fn << std::endl;

	//
	//	if(fp) //debug
	//	{
	//		std::stringstream ss;
	//		ss << "\t";
	//		uint32_t begin = 0;
	//		for(uint32_t i=begin; i<sendBuffer.size(); i++)
	//		{
	//			if(i==begin+2) ss << ":::";
	//			else if(i==begin+10) ss << ":::";
	//			ss << std::setfill('0') << std::setw(2) << std::hex << (((int16_t)
	// sendBuffer[i]) &0xFF) << "-" << std::dec;
	//		}
	//		ss << std::endl;
	//		fprintf(fp,"%s",ss.str().c_str());
	//	}
	//	if(fp) fclose(fp);

	if(TransceiverSocket::send(OtsUDPBoard_, sendBuffer, verbose_) < 0)
	{
		__SS__ << "Write failed to hardware at IP:Port " << OtsUDPBoard_.getIPAddress()
		       << ":" << OtsUDPBoard_.getPort() << __E__;
		__SS_THROW__;
	}
}
catch(std::runtime_error& e)
{
	throw;
}
catch(...)
{
	__SS__ << "Unrecognized exception caught!" << std::endl;
	try	{ throw; } //one more try to printout extra info
	catch(const std::exception &e)
	{
		ss << "Exception message: " << e.what();
	}
	catch(...){}
	__SS_THROW__;
}

//==============================================================================
void OtsUDPHardware::write(const std::vector<std::string>& sendBuffer)
{
	for(const auto& it : sendBuffer)
	{
		//		char msg[100];
		//		sprintf(msg,"_%d",getPort());
		//		std::string fn = "/tmp/new_udp_chk" + std::string(msg) + ".dat";
		//		FILE *fp = fopen(fn.c_str(),"a");
		//		__COUT__ << fn << std::endl;

		//		if(fp) //debug
		//		{
		//			std::stringstream ss;
		//			ss << "\t";
		//			uint32_t begin = 0;
		//			for(uint32_t i=begin; i<it.size(); i++)
		//			{
		//				if(i==begin+2) ss << ":::";
		//				else if(i==begin+10) ss << ":::";
		//				ss << std::setfill('0') << std::setw(2) << std::hex << (((const
		// int16_t)  it[i]) &0xFF) << "-" << std::dec;
		//			}
		//			ss << std::endl;
		//			fprintf(fp,"%s",ss.str().c_str());
		//		}
		//		if(fp) fclose(fp);

		write(it);
	}
}

//==============================================================================
void OtsUDPHardware::writeAndAcknowledge(const std::string& buffer, int timeoutSeconds)
try
{
	//	char msg[100];
	//	sprintf(msg,"_%d",getPort());
	//	std::string fn = "/tmp/new_udp_chk" + std::string(msg) + ".dat";
	//	FILE *fp = fopen(fn.c_str(),"a");
	//	__COUT__ << fn << std::endl;

	//	if(fp) //debug
	//	{
	//		std::stringstream ss;
	//		ss << "\tack ";
	//		uint32_t begin = 0;
	//		for(uint32_t i=begin; i<buffer.size(); i++)
	//		{
	//			if(i==begin+2) ss << ":::";
	//			else if(i==begin+10) ss << ":::";
	//			ss << std::setfill('0') << std::setw(2) << std::hex << (((int16_t)
	// buffer[i]) &0xFF) << "-" << std::dec;
	//		}
	//		ss << std::endl;
	//		fprintf(fp,"%s",ss.str().c_str());
	//	}
	//	if(fp) fclose(fp);

	//	__COUT__ << std::endl;
	//	for(auto& b: buffer)
	//	{
	//		__COUT__ << std::hex << (uint16_t)b << std::dec << std::endl;
	//	}
	TransceiverSocket::send(OtsUDPBoard_, buffer, verbose_);
	// acknowledgment_.clear();

	if(timeoutSeconds < 0)  // use default timeout
		timeoutSeconds = 5;

	if(TransceiverSocket::receive(acknowledgment_, timeoutSeconds) < 0)
	{
		__SS__ << "writeAndAcknowledge failed from hardware at IP:Port "
		       << OtsUDPBoard_.getIPAddress() << ":" << OtsUDPBoard_.getPort()
		       << ". Timeout period of " << timeoutSeconds
		       << " seconds reached without response." << std::endl;
		__COUT_ERR__ << "\n" << ss.str() << std::endl;
		__SS_THROW__;
	}

	//	__COUT__ << "Acknowledgment size: " << acknowledgment_.size() << std::endl;
	//	for(unsigned int i=0; i< acknowledgment_.size(); i++)
	//	{
	//		if(i%24 == 0)
	//		__COUT__ << i << "("<< (unsigned int)acknowledgment_[i] << ")-" << std::endl;
	//	}
	//

	// Check First Byte:
	// (https://docs.google.com/document/d/1i3Z07n8Jq78NwgUFdjAv2sLGhH4rWjHeYEScAWBzSyw/edit?usp=sharing)
	//	bits 2:0 = Packet Type (0: Read, 1: First in Burst, 2: Burst Middle, 3: Last in
	// Burst, 4: Read Ack, 5: Write Ack, 6: Burst Start Ack, 7: Burst Stop Ack) 	bit 3
	// =  ack of no address increment op 	bit 4 = err detected in protocol since reset
	//	bit 5 = rx overflow since reset
	//	bit 6 = crc err flag
	//	bit 7 = crc err detected

	if((acknowledgment_[0] >> 4))
	{
		__SS__ << "Error in OTS protocol encountered! " << std::setfill('0')
		       << std::setw(2) << std::hex << (((int16_t)acknowledgment_[0]) & 0xff)
		       << "-" << std::dec << __E__;
		__SS_THROW__;
	}
}
catch(std::runtime_error& e)
{
	throw;
}
catch(...)
{
	__SS__ << "Unrecognized exception caught!" << std::endl;
	try	{ throw; } //one more try to printout extra info
	catch(const std::exception &e)
	{
		ss << "Exception message: " << e.what();
	}
	catch(...){}
	__COUT_ERR__ << "\n" << ss.str() << std::endl;
	__SS_THROW__;
}

//==============================================================================
void OtsUDPHardware::writeAndAcknowledge(const std::vector<std::string>& buffer,
                                         int /*timeoutSeconds*/)
{
	for(const auto& it : buffer)
	{
		//		char msg[100];
		//		sprintf(msg,"_%d",getPort());
		//		std::string fn = "/tmp/new_udp_chk" + std::string(msg) + ".dat";
		//		FILE *fp = fopen(fn.c_str(),"a");
		//		__COUT__ << fn << std::endl;

		//		if(fp) //debug
		//		{
		//			std::stringstream ss;
		//			ss << "\tack ";
		//			uint32_t begin = 0;
		//			for(uint32_t i=begin; i<it.size(); i++)
		//			{
		//				if(i==begin+2) ss << ":::";
		//				else if(i==begin+10) ss << ":::";
		//				ss << std::setfill('0') << std::setw(2) << std::hex << (((const
		// int16_t)  it[i]) &0xFF) << "-" << std::dec;
		//			}
		//			ss << std::endl;
		//			fprintf(fp,"%s",ss.str().c_str());
		//		}
		//		if(fp) fclose(fp);

		writeAndAcknowledge(it);
	}
}

//==============================================================================
/// return -1 on failure
void OtsUDPHardware::read(const std::string& sendBuffer,
                          std::string&       receiveBuffer,
                          int                timeoutSeconds)
try
{
	{  // clear packets so that read matches!
		int clearedPackets = OtsUDPHardware::clearReadSocket();
		if(clearedPackets)
			__COUT__ << "Cleared receive socket buffer: " << clearedPackets
			         << " packets cleared." << std::endl;
	}

	__COUT__ << "Sending" << std::endl;
	TransceiverSocket::send(OtsUDPBoard_, sendBuffer, verbose_);

	if(timeoutSeconds < 0)  // use default timeout
		timeoutSeconds = 5;

	__COUT__ << "Receiving" << std::endl;
	if(TransceiverSocket::receive(
	       receiveBuffer, timeoutSeconds, 0 /*timeoutUSeconds*/, verbose_) < 0)
	{
		__SS__ << "Read failed from hardware at IP : Port " << OtsUDPBoard_.getIPAddress()
		       << " : " << OtsUDPBoard_.getPort() << ". Timeout period of "
		       << timeoutSeconds << " seconds reached without response." << __E__;
		__SS_THROW__;
	}

	//__COUT__ << "Received Message: ";
	// for(uint32_t i=0; i<receiveBuffer.size(); i++)
	//	std::cout << std::setfill('0') << std::setw(2) << std::hex << (((int16_t)
	// receiveBuffer[i]) &0xff) << "-" << std::dec;  std::cout << std::endl;

	// Check First Byte:
	// (https://docs.google.com/document/d/1i3Z07n8Jq78NwgUFdjAv2sLGhH4rWjHeYEScAWBzSyw/edit?usp=sharing)
	//	bits 2:0 = Packet Type (0: Read, 1: First in Burst, 2: Burst Middle, 3: Last in
	// Burst, 4: Read Ack, 5: Write Ack, 6: Burst Start Ack, 7: Burst Stop Ack) 	bit 3
	// =  ack of no address increment op 	bit 4 = err detected in protocol since reset
	//	bit 5 = rx overflow since reset
	//	bit 6 = crc err flag
	//	bit 7 = crc err detected

	if((receiveBuffer[0] >> 5))  // FIXME?? also consider bit 4?
	{
		__SS__ << "Error in OTS protocol encountered! " << std::setfill('0')
		       << std::setw(2) << std::hex << (((int16_t)receiveBuffer[0]) & 0xff) << "-"
		       << std::dec << __E__;
		//__SS_THROW__;
	}
}
catch(std::runtime_error& e)
{
	throw;
}
catch(...)
{
	__SS__ << "Unrecognized exception caught!" << std::endl;
	try	{ throw; } //one more try to printout extra info
	catch(const std::exception &e)
	{
		ss << "Exception message: " << e.what();
	}
	catch(...){}
	__SS_THROW__;
}

//==============================================================================
void OtsUDPHardware::read(const std::vector<std::string>& sendBuffers,
                          std::vector<std::string>&       receiveBuffers,
                          int /*timeoutSeconds*/)
{
	receiveBuffers.resize(sendBuffers.size());
	auto receiveBufferIterator = receiveBuffers.begin();
	for(const auto& sendBuffer : sendBuffers)
		read(sendBuffer, *(receiveBufferIterator++));
}

//==============================================================================
void OtsUDPHardware::read(const std::string& sendBuffer,
                          uint64_t&          receiveQuadWord,
                          int /*timeoutSeconds*/)
{
	std::string receiveBuffer;
	read(sendBuffer, receiveBuffer);

	// force response to be only one quad word
	if(receiveBuffer.length() != 10)
	{
		__SS__ << "Read uint64_t quad-word failed. Invalid size of received buffer: "
		       << receiveBuffer.length() << " != " << 10 << std::endl;
		__SS_THROW__;
	}
	// copy_n does not work!! alert?! it only copies the first byte?
	//	std::copy_n((char *)&receiveBuffer[2],sizeof(uint64_t),&receiveQuadWord);
	//
	//	__COUT__ << "receiveQuadWord all = 0x" << std::hex <<
	//			receiveQuadWord << std::dec << std::endl;

	receiveQuadWord = *((uint64_t*)&receiveBuffer[2]);
	//	//memcpy((char *)&receiveBuffer[2],sizeof(uint64_t),&receiveQuadWord);
	//	__COUT__ << "receiveQuadWord all = 0x" << std::hex <<
	//			receiveQuadWord << std::dec << std::endl;
}

//==============================================================================
void OtsUDPHardware::read(const std::string&     sendBuffer,
                          std::vector<uint64_t>& receiveQuadWords,
                          int /*timeoutSeconds*/)
{
	receiveQuadWords.resize(0);  // clear

	std::string receiveBuffer;
	read(sendBuffer, receiveBuffer);

	// force response to be only integer quad words
	if((receiveBuffer.length() - 2) % 8 != 0)
	{
		__SS__ << "Read vector of uint64_t quad-word failed. Invalid size of received "
		          "buffer: ("
		       << receiveBuffer.length() << " - 2) % 8 != 0" << std::endl;
		__SS_THROW__;
	}

	for(unsigned int i = 2; i < receiveBuffer.length(); i += 8)
	{
		receiveQuadWords.push_back(uint64_t());

		// copy_n does not work!! alert?! it only copies the first byte?
		// std::copy_n((char
		// *)&receiveBuffer[i],sizeof(uint64_t),&receiveQuadWords[receiveQuadWords.size()-1]);
		receiveQuadWords[receiveQuadWords.size() - 1] = *((uint64_t*)&receiveBuffer[i]);
	}
}

//==============================================================================
void OtsUDPHardware::read(const std::vector<std::string>&      sendBuffers,
                          std::vector<std::vector<uint64_t> >& receiveQuadWordsVector,
                          int /*timeoutSeconds*/)
{
	receiveQuadWordsVector.resize(
	    sendBuffers.size());  // create a return vector for each send buffer

	std::string receiveBuffer;

	// for each send buffere
	for(unsigned int b = 0; b < sendBuffers.size(); ++b)
	{
		receiveQuadWordsVector[b].resize(0);  // clear

		read(sendBuffers[b], receiveBuffer);

		// force response to be only integer quad words
		if((receiveBuffer.length() - 2) % 8 != 0)
		{
			__SS__ << "Read vector of uint64_t quad-word failed. Invalid size of "
			          "received buffer: ("
			       << receiveBuffer.length() << " - 2) % 8 != 0" << std::endl;
			__SS_THROW__;
		}

		// copy to uint64_t's
		for(unsigned int i = 2; i < receiveBuffer.length(); i += 8)
		{
			receiveQuadWordsVector[b].push_back(uint64_t());
			std::copy_n((char*)&receiveBuffer[i],
			            sizeof(uint64_t),
			            &receiveQuadWordsVector[b][receiveQuadWordsVector[b].size() - 1]);
		}
	}
}

//==============================================================================
/// clearReadSocket
///
///	flushes read socket.
///
///	reads from read socket until timeout is reached (remove stale packets)
/// returns count of packets that were cleared
int OtsUDPHardware::clearReadSocket()
{
	std::string dummerReceiveBuffer;
	int         cnt = 0;

	// receive with no timeout
	try
	{
		while(TransceiverSocket::receive(dummerReceiveBuffer,
		                                 0 /*timeoutSeconds*/,
		                                 0 /*timeoutUSeconds*/,
		                                 false /*verbose*/) >= 0)
			++cnt;
	}
	catch(...)
	{
		// ignore exceptions... assume due to there be nothing to read
		__COUT__ << "I am crashing while trying to read the socket...strange!"
		         << std::endl;
	}
	return cnt;
}

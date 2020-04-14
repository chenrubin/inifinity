#ifndef __DRIVER_PROXY__
#define __DRIVER_PROXY__

#include <boost/function.hpp>			// boost::function
#include <boost/thread.hpp>				// boost::thread
#include <linux/nbd.h>          		// nbd_request, nbd_reply

#include "../../cpp_fs/ws/reactor/reactor.hpp" 					// for class Reactor
#include "proxy_packet.hpp" 			// for ReplyPacket, RequestPacket

//------------------------------------------------------------------------------
namespace ilrd
{
//------------------------------------------------------------------------------
class DriverProxy
{
public:
	DriverProxy(Reactor *reactor_,
			boost::function<void(const RequestPacketRead&)> onRead_, 
            boost::function<void(const RequestPacketWrite&)> onWrite_);
	~DriverProxy();

	void ReplyRead(const ReplyPacketRead& packet_);
	void ReplyWrite(const ReplyPacketWrite& packet_);

private:
	// When proxy recieves nbd request packet it determines whether it is 
	// read or write. According to the type it creates the appropriate
	// request to the master 
	// throw bad_alloc
	void OnRequestIMP(int fd_);

	// once a key is pressed reactor stops
	void ReactorStopTaskIMP(int fd_);
	// Creates nbd fd for the device path
	int CreateNbdFdIMP();
	// Creates nbd clocking connection
	void NbdConnectionInitializerIMP(int fd, int sock);
	void CreateRequestPacketWriteIMP(int fd, RequestPacketWrite *packet, 
								  	 const nbd_request *nbdPacket);
	void CreateRequestPacketReadIMP(RequestPacketRead *packet, 
								 	const nbd_request *nbdPacket);
	void CreateReplyPacketWriteIMP(struct nbd_reply *nbdReply, 
								   const ReplyPacketWrite *packet_);
	void CreateReplyPacketReadIMP(struct nbd_reply *nbdReply, 
								  const ReplyPacketRead *packet_);
	// Insert data ('len' length) from buff into src 
	// one char at a time							 
	void InsertDataChunkToReqPacketIMP(std::vector<char> *src, 
									   size_t len, 
									   char *buff);

    Reactor *m_reactor;
	boost::function<void(const RequestPacketRead&)> m_onRead;
	boost::function<void(const RequestPacketWrite&)> m_onWrite;
	boost::thread m_thread_run;
	boost::thread m_thread_nbd;
	int m_sockPair[2];
	int m_nbdFd;
};
//------------------------------------------------------------------------------
} // namespace ilrd
//------------------------------------------------------------------------------

#endif  //__DRIVER_PROXY__









/*----------------------------------------------------------------------------*/

// the following lines are for convinience only and are not part of the header.

// nbd-proxy protocol
/*
struct nbd_request { // this packet is sent from nbd to proxy
	__be32 magic;
	__be32 type;	// == READ || == WRITE
	char handle[8];
	__be64 from;
	__be32 len;
} __attribute__((packed));

struct nbd_reply { // this packet is sent from proxy to nbd
	__be32 magic;
	__be32 error;		// 0 = ok, else error
	char handle[8];		// handle you got from request
};
*/
// minion-master protocol
/*
+ Send to minion:
  + type: 1 byte (0 - read, 1 - write) 
  + uid: 8 bytes (with network endianess)
  + block index: 8 bytes (with network endianess)
  + In case of write request - Data (4kb)

+ Reply from minion:
  + type: 1 byte (0 - read, 1 - write) 
  + uid: 8 bytes (with network endianess)
  + status: 1 byte
  + In case of read request - Data (4kb)
*/
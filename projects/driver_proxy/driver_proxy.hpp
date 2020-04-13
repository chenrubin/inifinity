#ifndef __DRIVER_PROXY__
#define __DRIVER_PROXY__

#include <boost/function.hpp>			// boost::function
#include <boost/thread.hpp>			// boost::thread

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
	void OnRequestIMP(int fd_);

	// once a key is pressed reactor stops
	void ReactorStopTaskIMP(int fd_);
	// Creates nbd fd for the device path
	int InitilizeNbdFdIMP();
	void NbdConnectionInitializerIMP(int fd, int sock);

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
struct nbd_request {
	__be32 magic;
	__be32 type;	// == READ || == WRITE
	char handle[8];
	__be64 from;
	__be32 len;
} __attribute__((packed));

struct nbd_reply {
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
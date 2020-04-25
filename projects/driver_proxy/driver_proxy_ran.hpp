#ifndef __DRIVER_PROXY__
#define __DRIVER_PROXY__

#include <boost/thread.hpp> //boost::thread

#include "reactor.hpp" // for class Reactor
#include "proxy_packet.hpp" // for ReplyPacket, RequestPacket
/*Read Me
* sudo modprobe nbd
* echo 4 | sudo tee /sys/block/nbd0/queue/max_sectors_kb
* sudo mkfs.ext4 /dev/nbd0
* sudo mount /dev/nbd0 /mnt
*/
//------------------------------------------------------------------------------
namespace ilrd
{
#ifndef BLOCK_SIZE
#define BLOCK_SIZE 4096
#endif //BLOCK_SIZE
#ifndef NUMBER_OF_BLOCKS
#define NUMBER_OF_BLOCKS 128
#endif //NUMBER_OF_BLOCKS
//------------------------------------------------------------------------------
class DriverProxy
{
public:
	DriverProxy(Reactor *reactor_,
			const boost::function<void(const RequestPacketRead&)> onRead_, 
            const boost::function<void(const RequestPacketWrite&)> onWrite_);
	~DriverProxy();

	void ReplyRead(const ReplyPacketRead& packet_);
	void ReplyWrite(const ReplyPacketWrite& packet_);

private:
	void OnRequestIMP(int fd_);
	int InitNbdSocketIMP();
	int* InitSocketPairIMP();
	void ThreadFuncIMP();
	int FlushNbdSocketIMP();
	void CloseSocketPair();

    Reactor *m_reactor;
	const boost::function<void(const RequestPacketRead&)> m_onRead;
	const boost::function<void(const RequestPacketWrite&)> m_onWrite;
	int *m_socketPair;
	int m_nbdSocket;
	boost::thread m_doWorker;
};
//------------------------------------------------------------------------------
} // namespace ilrd
//------------------------------------------------------------------------------

#endif  //__DRIVER_PROXY__

/*----------------------------------------------------------------------------*/

/* the following lines are for convinience only and are not part of the header.
 * nbd-proxy protocol

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

// minion-master protocol

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
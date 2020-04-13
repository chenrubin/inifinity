// NBD Proxy - Header file
// Last update: 8/4/2020
// Author: 
/*----------------------------------------------------------------------------*/
#ifndef __NBD_PROXY__
#define __NBD_PROXY__
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/

class NBDProxy
{
public:
	NBDProxy(Reactor *reactor_, const boost::function<void(const RequestPacket&)> onRead_, 
                    const boost::function<void(const RequestPacket&)> onWrite_)
	~NBDProxy();

	OnReplyRead(const ReplyPacket& packet_);
	OnReplyWrite(const ReplyPacket& packet_);

private:
	OnRequest();

    Reactor *m_reactor;
	const boost::function<void(const RequestPacket&)> m_onRead;
	const boost::function<void(const RequestPacket&)> m_onWrite;
};

/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif  //__NBD_PROXY__
/*----------------------------------------------------------------------------*/

// the following lines are for convinience only and are not part of the header.

// nbd-proxy protocol

struct nbd_request {
	__be32 magic;
	__be32 type;	/* == READ || == WRITE 	*/
	char handle[8];
	__be64 from;
	__be32 len;
} __attribute__((packed));

struct nbd_reply {
	__be32 magic;
	__be32 error;		/* 0 = ok, else error	*/
	char handle[8];		/* handle you got from request	*/
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

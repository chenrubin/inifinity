#include <cassert> //assert
#include <iostream> //cout
#include <fcntl.h> //open /O_RDWR
#include <sys/socket.h> //socketpair / AF_UNIX / SOCK_STREAM
#include <linux/nbd.h> //NBD_XXX
#include <sys/ioctl.h> //ioctl / _IO
#include <boost/bind.hpp> //bind()
#include <errno.h> //errno
#include <cstdio> //perror
#include <signal.h> // sigfillset / SIG_SETMASK / sigprocmask
#include <vector> //std::vector
#include <algorithm> //reversr
#include <arpa/inet.h> //ntohl

#include "driver_proxy.hpp" // driver packet / reactor header/ proxy_packet
#include "myutils.hpp" //UNUSED

//------------------------------------------------------------------------------
namespace
{
//------------------------------------------------------------------------------
int ReadFromFdIMP(int fd_, void *source_, size_t sizeOfSource_)
{
	DEBUG_MSG("10.in DriverProxy - ReadFromFdIMP");
	int result = 1;
	size_t leftToRead = sizeOfSource_;
	char *readHeaderRunner = (char*)(source_);
	while(0 < leftToRead || 0 == result)
	{
		result = read(fd_, readHeaderRunner, leftToRead);
		if(-1 == result)
		{
			break;
		}
		leftToRead -= result;
		readHeaderRunner += result;
	}
	DEBUG_MSG("10.1result")
	DEBUG_MSG(result)
	return result;
}
//--------------------------------------------------------------------
int WriteToFdIMP(int fd_, void *source_, size_t sizeOfSource_)
{
	DEBUG_MSG("in DriverProxy - WriteToFdIMP");
	int result = 0;
	size_t leftToWrite = sizeOfSource_;
	char *readHeaderRunner = (char*)(source_);
	while(0 < leftToWrite || result == 0)
	{
		result = write(fd_, readHeaderRunner, leftToWrite);
		if(-1 == result)
		{
			break;
		}
		leftToWrite -= result;
		readHeaderRunner += result;
	}

	return result;
}
//--------------------------------------------------------------------
int WriteVectorToFdIMP(int fd_, const std::vector<char> *data_, size_t len_)
{
	DEBUG_MSG("in DriverProxy - WriteVectorToFdIMP");
	DEBUG_MSG("the len is:");
	DEBUG_MSG(len_);
	int result = 0;
	result = write(fd_, &((*data_)[0]), len_);

		if(-1 == result)
		{
			perror("WriteVectorToFdIMP - write");
			DEBUG_MSG("errno:");
			DEBUG_MSG(errno);
		}
	DEBUG_MSG("the result is:");
	DEBUG_MSG(result);

	return result;
}
//------------------------------------------------------------------------------
void InitNbdReply(nbd_reply* reply_)
{
	reply_->error = 0;
	reply_->handle[0] = 0;
	reply_->handle[1] = 0;
	reply_->handle[2] = 0;
	reply_->handle[3] = 0;
	reply_->handle[4] = 0;
	reply_->handle[5] = 0;
	reply_->handle[6] = 0;
	reply_->handle[7] = 0;
	reply_->magic = 0;
}
//------------------------------------------------------------------------------
} //end unonimus namespace
//------------------------------------------------------------------------------
namespace ilrd
{
//------------------------------------------------------------------------------
DriverProxy:: DriverProxy(Reactor *reactor_,
			const boost::function<void(const RequestPacketRead&)> onRead_, 
            const boost::function<void(const RequestPacketWrite&)> onWrite_)
: m_reactor(reactor_)
, m_onRead(onRead_)
, m_onWrite(onWrite_)
, m_socketPair(InitSocketPairIMP())
, m_nbdSocket(InitNbdSocketIMP())
, m_doWorker(&DriverProxy::ThreadFuncIMP,this)
{
	DEBUG_MSG("6. in DriverProxy - Ctor");
	boost::function<void(int)> func = 
	boost::bind(&DriverProxy::OnRequestIMP, this, m_socketPair[0]);
	m_reactor->AddFd(m_socketPair[0], Reactor::READ, func);

	assert(!m_onWrite.empty());
	assert(!m_onRead.empty());
}
//------------------------------------------------------------------------------
DriverProxy::~DriverProxy()
{
	DEBUG_MSG("in DriverProxy - Dtor");
	FlushNbdSocketIMP();
	m_doWorker.join();
	CloseSocketPair();
	delete[] m_socketPair;
}
//------------------------------------------------------------------------------
void DriverProxy::ReplyRead(const ReplyPacketRead& packet_)
{
	DEBUG_MSG("in DriverProxy - ReplayRead");

	struct nbd_reply nbdReplyRead;
	InitNbdReply(&nbdReplyRead);

	nbdReplyRead.magic = htonl(NBD_REPLY_MAGIC);
	memcpy(nbdReplyRead.handle, &(packet_.uid), sizeof(nbdReplyRead.handle));
	nbdReplyRead.error = htonl(packet_.status);
	DEBUG_MSG("in DriverProxy - ReplayRead - nbdReplyRead.error:");
	DEBUG_MSG(nbdReplyRead.error);
	DEBUG_MSG("in DriverProxy - ReplayRead - nbdReplyRead.handle:");
	DEBUG_MSG(nbdReplyRead.handle);
	DEBUG_MSG("in DriverProxy - ReplayRead - nbdReplyRead.magic:");
	DEBUG_MSG(nbdReplyRead.magic);

	WriteToFdIMP(m_socketPair[0],&nbdReplyRead, sizeof(nbdReplyRead));
	WriteVectorToFdIMP(m_socketPair[0], &packet_.data, packet_.len);
}
//------------------------------------------------------------------------------
void DriverProxy::ReplyWrite(const ReplyPacketWrite& packet_)
{
	DEBUG_MSG("in DriverProxy - ReplyWrite");

	struct nbd_reply nbdReplyWrite;
	InitNbdReply(&nbdReplyWrite);
	
	nbdReplyWrite.magic = htonl(NBD_REPLY_MAGIC);
	DEBUG_MSG("before memcpy")
	memcpy(nbdReplyWrite.handle, &(packet_.uid), sizeof(nbdReplyWrite.handle));
	DEBUG_MSG("after memcpy")
	nbdReplyWrite.error = htonl(packet_.status);
	//write(m_socketPair[0], &nbdReplyWrite, sizeof(nbdReplyWrite));
	WriteToFdIMP(m_socketPair[0],&nbdReplyWrite, sizeof(nbdReplyWrite));
}
//------------------------------------------------------------------------------
void DriverProxy::OnRequestIMP(int fd_)
{
	DEBUG_MSG("9. in DriverProxy - OnRequest");
	struct nbd_request nbdHeaderBuffer;
	RequestPacketWrite reqPacketWrite;
	//read request
	if(-1 == ReadFromFdIMP(m_socketPair[0]
			, &nbdHeaderBuffer, sizeof(nbdHeaderBuffer)))
	{
		perror("ReadFromFd");
		DEBUG_MSG("the errno is:" + errno)
	}

	nbdHeaderBuffer.len = ntohl(nbdHeaderBuffer.len);
	nbdHeaderBuffer.magic = ntohl(nbdHeaderBuffer.magic);
	nbdHeaderBuffer.type = ntohl(nbdHeaderBuffer.type);
	nbdHeaderBuffer.from =  be64toh(nbdHeaderBuffer.from);

	if(NBD_REQUEST_MAGIC != nbdHeaderBuffer.magic)
	{
		DEBUG_MSG("!!9.13. in DriverProxy - OnRequestIMP - Wrong magic number");
		return;
	}
	//resolve request
	switch(nbdHeaderBuffer.type)
	{
		case NBD_CMD_READ:
			DEBUG_MSG("case NBD_CMD_READ")
			RequestPacketRead reqPacketRead;
			reqPacketRead.len = nbdHeaderBuffer.len;
			memcpy(&reqPacketRead.uid, nbdHeaderBuffer.handle
					, sizeof(nbdHeaderBuffer.handle));
			reqPacketRead.offset = nbdHeaderBuffer.from;
			assert(!m_onRead.empty());
			m_onRead(reqPacketRead);
		break;

		case NBD_CMD_WRITE:
			DEBUG_MSG("9.13. case NBD_CMD_WRITE")
			reqPacketWrite.len = nbdHeaderBuffer.len;
			memcpy(&reqPacketWrite.uid, nbdHeaderBuffer.handle
					, sizeof(nbdHeaderBuffer.handle));
			reqPacketWrite.offset = nbdHeaderBuffer.from;
			//read data
			reqPacketWrite.data.reserve(nbdHeaderBuffer.len);
			ReadFromFdIMP(m_socketPair[0], &(reqPacketWrite.data[0])
					, nbdHeaderBuffer.len);
			// activate OnWrite
			DEBUG_MSG("9.14. before OnWirte")
			assert(!m_onWrite.empty());
			m_onWrite(reqPacketWrite);
			DEBUG_MSG("9.15. after OnWirte")
		break;
		case NBD_CMD_DISC:
			DEBUG_MSG("case NBD_CMD_DISC")
			CloseSocketPair();
		break;
		case NBD_CMD_FLUSH:
			DEBUG_MSG("case NBD_CMD_FLUSH")
		break;

		default:
			DEBUG_MSG("case default")
		break;
	}
}
//------------------------------------------------------------------------------
int DriverProxy::InitNbdSocketIMP()
{
	DEBUG_MSG("4. in DriverProxy - InitNbdSocketIMP");
	int return_nbdSocket = open("/dev/nbd0", O_RDWR);
  	if (-1 == return_nbdSocket)
	{
    	perror("4.1.Failed to open /dev/nbd0");
		DEBUG_MSG(errno)
	}
	//config nbd
    int err = ioctl(return_nbdSocket, NBD_SET_BLKSIZE, BLOCK_SIZE);
    if (-1 == err)
	{
    	perror("4.2.Failed to set Block Size");
		DEBUG_MSG(errno)
	}

	err = ioctl(return_nbdSocket, NBD_SET_SIZE_BLOCKS, NUMBER_OF_BLOCKS);
    if (-1 == err)
	{
    	perror("4.3Failed to set NUmber OF Blocks");
		DEBUG_MSG(errno)
	}

	err = ioctl(return_nbdSocket, NBD_CLEAR_SOCK);
	if (-1 == err)
	{
    	perror("4.4Failed to Clear Sock");
		DEBUG_MSG(errno);
	}

	return return_nbdSocket;
}
//------------------------------------------------------------------------------
int* DriverProxy::InitSocketPairIMP()
{
	DEBUG_MSG("3. in DriverProxy - InitSocketPairIMP");
	int* returnPair = new int[2];
	int err = socketpair(AF_UNIX, SOCK_STREAM, 0, returnPair);
	if(-1 == err)
	{
		perror("3.1.creat socketpair");
		DEBUG_MSG("3.2.errno: ")
		DEBUG_MSG(errno)
	}
	return returnPair;
}
//------------------------------------------------------------------------------
void DriverProxy::ThreadFuncIMP()
{
	DEBUG_MSG("5. in DriverProxy - ThreadFuncIMP");
	sigset_t sigset;
    if (0 != sigfillset(&sigset) ||
        0 != sigprocmask(SIG_SETMASK, &sigset, NULL))
	{
      std::cout << "failed to block signals in thread" << std::endl;
    }
    int userEndSocket = m_socketPair[1];

    if(-1 == ioctl(m_nbdSocket, NBD_SET_SOCK, userEndSocket))
	{
		perror("nbd_set_socket");
		DEBUG_MSG("errno: ")
		DEBUG_MSG(errno)
    }
    else
	{
		int flags = 0;
		//need to add proper response for the FLUSH and TRIM
	// #ifdef NBD_FLAG_SEND_FLUSH
    // 	flags |= NBD_FLAG_SEND_FLUSH;
	// #endif
	// #ifdef NBD_FLAG_SEND_TRIM
	// 	flags |= NBD_FLAG_SEND_TRIM;
	// #endif
      	if (0 != flags && -1 == ioctl(m_nbdSocket, NBD_SET_FLAGS, flags))
	  	{
        	perror("NBD_SET_FLAGS");
			DEBUG_MSG("errno: ")
			DEBUG_MSG(errno)
      	}
      int err = ioctl(m_nbdSocket, NBD_DO_IT);
      if (-1 == err)
	  {
		  perror("NBD_DO_IT terminated with error");
		  DEBUG_MSG("errno: ")
		  DEBUG_MSG(errno)
      }
    }

    if (-1 == FlushNbdSocketIMP())
	{
      std::cout << "failed to perform nbd cleanup actions" << std::endl;
    }
}
//------------------------------------------------------------------------------
int DriverProxy::FlushNbdSocketIMP()
{
	DEBUG_MSG("in DriverProxy - FlushNbdSocketIMP");
	return (-1 == -ioctl(m_nbdSocket, NBD_CLEAR_QUE)
			|| -1 == ioctl(m_nbdSocket, NBD_CLEAR_SOCK));
}
//------------------------------------------------------------------------------
void DriverProxy::CloseSocketPair()
{
	DEBUG_MSG("in DriverProxy - CloseSocketPair");
	close(m_socketPair[0]);
	close(m_socketPair[1]);
}
//------------------------------------------------------------------------------
} // namespace ilrd
//------------------------------------------------------------------------------

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
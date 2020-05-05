#include <sys/socket.h>         // socketpair 
#include <boost/bind.hpp>       // boost::bind
#include <fcntl.h>              // O_RDWR
#include <sys/ioctl.h>          // ioctl
#include <arpa/inet.h>          // htonl

#include "driver_proxy.hpp"
#include "MyUtils.hpp"          // HandleErrorIfExists
#include "logger.hpp"           // //LOG_DEBUG...

#define STDIN (0)
#define BLOCK_SIZE 4096
#define NUM_OF_BLOCKS 128
#define PATH_LEN 100

namespace
{
template <typename T>    
void ReverseEndianessIMP(T *num1);
void swapIMP(char *num1, char *num2);
void InitiateLogger(); 
}

namespace ilrd
{
void WriteAllIMP(int fd, char *buff, size_t count);

typedef boost::function<void(const RequestPacketRead&)> read_func_t;
typedef boost::function<void(const RequestPacketWrite&)> write_func_t;

DriverProxy::DriverProxy(Reactor *reactor_, 
                         read_func_t onRead_,
                         write_func_t onWrite_)
    : m_reactor(reactor_)
    , m_onRead(onRead_)
    , m_onWrite(onWrite_)
    , m_thread_nbd()
    , m_sockPair()
    , m_nbdFd(CreateNbdFdIMP())
{
    InitiateLogger();
    //LOG_DEBUG("inside Proxy Ctor");
    HandleErrorIfExists(socketpair(AF_UNIX, SOCK_STREAM, 0, m_sockPair),
                        "create socket pair");
    ConfigureNbdSizeIMP();
    AddFdsToReactorIMP();

    m_thread_nbd = boost::thread(&DriverProxy::NbdConnectionInitializerIMP, 
                                 this);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
DriverProxy::~DriverProxy()
{
    m_thread_nbd.join();
    close(m_sockPair[1]);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::ReplyRead(const ReplyPacketRead& packet_)
{
    struct nbd_reply nbdReply;
    //LOG_DEBUG("inside reply read");
    std::cout << "inside ReplyRead\n";
    CreateReplyPacketReadIMP(&nbdReply, &packet_);
    WriteAllIMP(m_sockPair[0], (char *)&nbdReply, sizeof(nbdReply));
#if NDEBUG
    std::cout << "inside reply read vectro size = " << packet_.data.size() << "\n";
#endif
    std::cout << "before writeall packet.len = " << packet_.len << "\n";
    WriteAllIMP(m_sockPair[0], (char *)&packet_.data[0], packet_.len);
    //LOG_DEBUG("end of reply read");
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::ReplyWrite(const ReplyPacketWrite& packet_)
{
    struct nbd_reply nbdReply;
    //LOG_DEBUG("inside ReplyWrite");

    CreateReplyPacketWriteIMP(&nbdReply, &packet_);
    WriteAllIMP(m_sockPair[0], (char *)&nbdReply, sizeof(nbdReply));

    //LOG_DEBUG("end of ReplyWrite");
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateReplyPacketReadIMP(struct nbd_reply *nbdReply, 
								          const ReplyPacketRead *packet_)
{
    std::cout << "inside CreateReplyPacketReadIMP\n";
    //LOG_DEBUG("inside CreateReplyPacketReadIMP");
    nbdReply->magic = htonl(NBD_REPLY_MAGIC);
    nbdReply->error = packet_->status;
    ReverseEndianessIMP(&nbdReply->error);
    memcpy(&nbdReply->handle, &packet_->uid, sizeof(nbdReply->handle));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateReplyPacketWriteIMP(struct nbd_reply *nbdReply, 
								            const ReplyPacketWrite *packet_)
{
    //LOG_DEBUG("inside CreateReplyPacketWritreIMP");
    nbdReply->magic = htonl(NBD_REPLY_MAGIC);
    nbdReply->error = packet_->status;
    ReverseEndianessIMP(&nbdReply->error);
    memcpy(&nbdReply->handle, &packet_->uid, sizeof(nbdReply->handle));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::OnRequestIMP(int fd_)
{
    size_t bytes_read = 0;
    std::cout << "std::cout inside OnRequestIMP before logger\n";
    //LOG_DEBUG("Inside OnRequestIMP");
    std::cout << "std::cout inside OnRequestIMP After logger\n";
    struct nbd_request nbdRequest;

    bytes_read = read(fd_, &nbdRequest, sizeof(nbdRequest));

    HandleErrorIfExists(bytes_read, "error reading user side of nbd socket");
    HandleErrorIfExists(true == (nbdRequest.magic == htonl(NBD_REQUEST_MAGIC)) ?
                            0 : -1, "Request magic number is illegal");
    int type = htonl(nbdRequest.type);

    switch (type)
    {
        case NBD_CMD_WRITE:
        {
            std::cout << "Inside NBD_CMD_WRITE\n";
            //LOG_DEBUG("Inside NBD_CMD_WRITE");
            RequestPacketWrite reqWrite;
            CreateRequestPacketWriteIMP(fd_, &reqWrite, &nbdRequest);
            m_onWrite(reqWrite);
            break;
        }
        case NBD_CMD_READ:
        {
            std::cout << "Inside NBD_CMD_READ\n";
            //LOG_DEBUG("Inside NBD_CMD_READ");
            RequestPacketRead reqRead;
            CreateRequestPacketReadIMP(&reqRead, &nbdRequest);
            m_onRead(reqRead);
            break;
        }
        case NBD_CMD_DISC:
        {
            //LOG_DEBUG("Inside NBD_CMD_DISC");
            m_reactor->RemoveFd(m_sockPair[0], m_reactor->READ);
            close(m_sockPair[0]);
            break;
        }
    } 
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateRequestPacketReadIMP(RequestPacketRead *packet, 
                                             const nbd_request *nbdPacket)
{
    //LOG_DEBUG("Inside CreateRequestPacketReadIMP");
    CreateRequestPacket(packet, nbdPacket);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateRequestPacketWriteIMP(int fd, 
                                              RequestPacketWrite *packet,
                                              const nbd_request *nbdPacket)
{
    //LOG_DEBUG("Inside CreateRequestPacketWriteIMP");
    size_t bytes_read = 0;
    CreateRequestPacket(packet, nbdPacket);

    char *buff = new char[packet->len];
    boost::shared_ptr<char> buff_ptr(buff);
    
    packet->data.resize(packet->len);
    bytes_read = read(fd, &packet->data[0], packet->len);
    packet->data.resize(packet->len);

    HandleErrorIfExists(bytes_read, "error reading user side of nbd socket");
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
template <typename T>
void DriverProxy::CreateRequestPacket(T *packet, 
                                      const nbd_request *nbdPacket)
{
    memcpy(&packet->uid, &nbdPacket->handle, sizeof(uint64_t));
    packet->offset = nbdPacket->from;
    ReverseEndianessIMP(&packet->offset);
    packet->len = htonl(nbdPacket->len);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::DisconnectSystemIMP(int fd_)
{
    //LOG_DEBUG("inside DisconnectSystemIMP");
    char buff[20];
    std::fgets(buff, sizeof(buff), stdin);
    if (0 == strcmp(buff, "stop\n"))
    {
        //LOG_DEBUG("inside stop");
        m_reactor->Stop();
    }
    else if (0 == strcmp(buff, "disconnect\n"))
    {
        //LOG_DEBUG("inside disconnect");
        HandleErrorIfExists(ioctl(m_nbdFd, NBD_DISCONNECT), 
                                  "Failed to Disconnect from nbd");
    }
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int DriverProxy::CreateNbdFdIMP()
{
    int nbd = open("/dev/nbd0", O_RDWR);
    HandleErrorIfExists(nbd, "create fd for nbd device path");

    return nbd;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::NbdConnectionInitializerIMP(/*int fd, int sock*/)
{
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_SET_SOCK, m_sockPair[1]), 
                        "nbd set socket failed");
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_DO_IT), "nbd do it failed");

    HandleErrorIfExists(ioctl(m_nbdFd, NBD_CLEAR_QUE), "failed to Clear queue"); 
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_CLEAR_SOCK), "failed to clear sock");
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_SET_FLAGS, 0), "failed to set flags");
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::AddFdsToReactorIMP()
{
    m_reactor->AddFd(m_sockPair[0], 
                     m_reactor->READ ,
                     boost::bind(&DriverProxy::OnRequestIMP, this, m_sockPair[0]));
            
    m_reactor->AddFd(STDIN, 
                     m_reactor->READ ,
                     boost::bind(&DriverProxy::DisconnectSystemIMP, this, STDIN));
}

void DriverProxy::ConfigureNbdSizeIMP()
{
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_SET_BLKSIZE, BLOCK_SIZE), 
                              "Failed to set blockSize");
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_SET_SIZE_BLOCKS, NUM_OF_BLOCKS), 
                        "Failed to set num of blocks");
}
/******************************************************************************
            Auxilary functions outside of class DriverProxy
******************************************************************************/
void WriteAllIMP(int fd, char *buff, size_t count)
{
    std::cout << "inside WriteAllIMP\n";
    ssize_t bytes_written = 0;
    std::cout << "count = " << count << "\n";
    while (count > 0)
    {
        std::cout << "Inside While count\n";
        std::cout << "about to write to fd " << fd << "\n";
        std::cout << "fcntl = " << fcntl(fd, F_GETFD) << "\n";
        bytes_written = write(fd, buff, count);

        std::cout << "Inside While count after write\n";
        HandleErrorIfExists(bytes_written, "Failed to write to buffer");
        buff += bytes_written;
        count -= bytes_written;
    }
    std::cout << "End of WriteAllIMP\n";
}
} // end of namespace ilrd
namespace
{
void InitiateLogger()
{
    char buf[PATH_LEN];
    std::string str = getcwd(buf, PATH_LEN);
    str += "/logger.txt"; 
    setenv("LOGGER_FILE", str.c_str(), 1);
}

template <typename T> 
void ReverseEndianessIMP(T *num1)
{
    size_t byteSize = sizeof(T);
    for (size_t i = 0; i < (byteSize / 2); ++i)
    {
        swapIMP((char *)num1 + i, (char *)num1 + byteSize - 1 - i);
    }
}

void swapIMP(char *num1, char *num2)
{
    char temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}
} // end of namespace
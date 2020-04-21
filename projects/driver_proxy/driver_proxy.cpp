#include <sys/socket.h>         // socketpair 
#include <boost/bind.hpp>       // boost::bind
#include <fcntl.h>              // O_RDWR
#include <sys/ioctl.h>          // ioctl
#include <arpa/inet.h>          // htonl

#include "driver_proxy.hpp"
#include "MyUtils.hpp"          // HandleErrorIfExists
#include "logger.hpp"           // //LOG_DEBUG...

#define STDIN (0)

namespace ilrd
{
void WriteAllIMP(int fd, char *buff, size_t count);
void ReverseEndianessIMP1(uint64_t *num1);
void ReverseEndianessIMP2(unsigned int *num1);
void swapIMP(char *num1, char *num2);


typedef boost::function<void(const RequestPacketRead&)> read_func_t;
typedef boost::function<void(const RequestPacketWrite&)> write_func_t;

DriverProxy::DriverProxy(Reactor *reactor_, 
                         read_func_t onRead_,
                         write_func_t onWrite_)
    : m_reactor(reactor_)
    , m_onRead(onRead_)
    , m_onWrite(onWrite_)
    , m_thread_run()
    , m_thread_nbd()
    , m_sockPair()
    , m_nbdFd(CreateNbdFdIMP())
{
    setenv("LOGGER_FILE", "/home/chenr/git/projects/driver_proxy/logger.txt", 1);
    HandleErrorIfExists(0 == socketpair(AF_UNIX, SOCK_STREAM, 0, m_sockPair) ? 
                        0 : -1, "create socket pair");
    ConfigureNbdSizeIMP();
    AddFdsToReactorIMP();                   

    m_thread_nbd = boost::thread(&DriverProxy::NbdConnectionInitializerIMP, 
                                 this, 
                                 m_nbdFd, 
                                 m_sockPair[1]);
    m_thread_run = boost::thread(&Reactor::Run, m_reactor);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
DriverProxy::~DriverProxy()
{
    m_thread_run.join();
    m_thread_nbd.join();
    close(m_sockPair[0]);
    close(m_sockPair[1]);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::ReplyRead(const ReplyPacketRead& packet_)
{
    struct nbd_reply nbdReply;
    //LOG_DEBUG("inside reply read");

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
    //LOG_DEBUG("inside CreateReplyPacketReadIMP");
    nbdReply->magic = htonl(NBD_REPLY_MAGIC);
    nbdReply->error = packet_->status;
    ReverseEndianessIMP2(&nbdReply->error);
    memcpy(&nbdReply->handle, &packet_->uid, sizeof(nbdReply->handle));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateReplyPacketWriteIMP(struct nbd_reply *nbdReply, 
								            const ReplyPacketWrite *packet_)
{
    //LOG_DEBUG("inside CreateReplyPacketWritreIMP");
    nbdReply->magic = htonl(NBD_REPLY_MAGIC);
    nbdReply->error = packet_->status;
    ReverseEndianessIMP2(&nbdReply->error);
    memcpy(&nbdReply->handle, &packet_->uid, sizeof(nbdReply->handle));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::OnRequestIMP(int fd_)
{
    size_t bytes_read = 0;
    std::cout << "std::cout inside OnRequestIMP before logger\n";
    LOG_DEBUG("Inside OnRequestIMP");
    std::cout << "std::cout inside OnRequestIMP After logger\n";
    struct nbd_request nbdRequest;

    read(fd_, &nbdRequest, sizeof(nbdRequest));
    HandleErrorIfExists(true == (nbdRequest.magic == htonl(NBD_REQUEST_MAGIC)) ?
                            0 : -1, "Request magic number is illegal");
    int type = htonl(nbdRequest.type);

    switch (type)
    {
        case NBD_CMD_WRITE:
        {
            //LOG_DEBUG("Inside NBD_CMD_WRITE");
            RequestPacketWrite reqWrite;
            CreateRequestPacketWriteIMP(fd_, &reqWrite, &nbdRequest);
            m_onWrite(reqWrite);
            break;
        }
        case NBD_CMD_READ:
        {
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

    HandleErrorIfExists(bytes_read, "error reading user side of nbd socket");
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateRequestPacketReadIMP(RequestPacketRead *packet, 
                                             const nbd_request *nbdPacket)
{
    //LOG_DEBUG("Inside CreateRequestPacketReadIMP");
    memcpy(&packet->uid, &nbdPacket->handle, sizeof(uint64_t));
    packet->offset = nbdPacket->from;
    ReverseEndianessIMP1(&packet->offset);
    packet->len = htonl(nbdPacket->len);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateRequestPacketWriteIMP(int fd, 
                                              RequestPacketWrite *packet,
                                              const nbd_request *nbdPacket)
{
    //LOG_DEBUG("Inside CreateRequestPacketWriteIMP");
    size_t bytes_read = 0;
    memcpy(&packet->uid, &nbdPacket->handle, sizeof(uint64_t));
    packet->offset = nbdPacket->from;
    ReverseEndianessIMP1(&packet->offset);
    packet->len = htonl(nbdPacket->len);

    char *buff = new char[packet->len];
    boost::shared_ptr<char> buff_ptr(buff);

    read(fd, buff_ptr.get(), packet->len);
    InsertDataChunkToReqPacketIMP(&packet->data, bytes_read, buff_ptr.get());
    
    HandleErrorIfExists(bytes_read, "error reading user side of nbd socket");
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::InsertDataChunkToReqPacketIMP(std::vector<char> *src, 
									            size_t len, 
									            char *buff)
{
    //LOG_DEBUG("Inside InsertDataChunkToReqPacketIMP");
    for (size_t i = 0; i < len; ++i)
    {
        src->push_back(buff[i]);
    }
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::DisconnectSystemIMP(int fd_)
{
    //LOG_DEBUG("inside DisconnectSystemIMP");
    char buff[20];
    fgets(buff, sizeof(buff), stdin);
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
void DriverProxy::NbdConnectionInitializerIMP(int fd, int sock)
{
    HandleErrorIfExists(ioctl(fd, NBD_SET_SOCK, sock), "nbd set socket failed");
    HandleErrorIfExists(ioctl(fd, NBD_DO_IT), "nbd do it failed");

    HandleErrorIfExists(ioctl(fd, NBD_CLEAR_QUE), "failed to Clear queue"); 
    HandleErrorIfExists(ioctl(fd, NBD_CLEAR_SOCK), "failed to clear sock"); 
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
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_SET_BLKSIZE, 4096), 
                              "Failed to set blockSize");
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_SET_SIZE_BLOCKS, 128), 
                        "Failed to set num of blocks");
}
/******************************************************************************
            Auxilary functions outside of class DriverProxy
******************************************************************************/
void WriteAllIMP(int fd, char *buff, size_t count)
{
    size_t bytes_written = 0;
    while (count > 0)
    {
    //    std::cout << "buf = " << buff << "\n";
        bytes_written = write(fd, buff, count);
        HandleErrorIfExists(bytes_written, "Failed to write to buffer");
        buff += bytes_written;
        count -= bytes_written;
    }
}

void ReverseEndianessIMP1(uint64_t *num1)
{
    for (size_t i = 0; i < 4; ++i)
    {
        swapIMP((char *)num1 + i, (char *)num1 + 7 - i);
    }
}

void ReverseEndianessIMP2(unsigned int *num1)
{
    for (size_t i = 0; i < 2; ++i)
    {
        swapIMP((char *)num1 + i, (char *)num1 + 3 - i);
    }
}

void swapIMP(char *num1, char *num2)
{
    char temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

} // end of namespace ilrd
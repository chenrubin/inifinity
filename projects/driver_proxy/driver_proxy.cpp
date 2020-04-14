#include <sys/socket.h>         // socketpair 
#include <boost/bind.hpp>       // boost::bind
#include <fcntl.h>              // O_RDWR
#include <sys/ioctl.h>          // ioctl
#include <arpa/inet.h>          // htonl

#include "driver_proxy.hpp"
#include "MyUtils.hpp"          // HandleErrorIfExists

#define STDIN (0)

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
    , m_thread_run()
    , m_thread_nbd()
    , m_sockPair()
    , m_nbdFd(CreateNbdFdIMP())
{
    HandleErrorIfExists(0 == socketpair(AF_UNIX, SOCK_STREAM, 0, m_sockPair) ? 
                        0 : -1, "create socket pair");
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_SET_BLKSIZE, 4096), 
                        "Failed to set blockSize");
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_SET_SIZE_BLOCKS, 128), 
                        "Failed to set num of blocks");
    m_reactor->AddFd(m_sockPair[0], 
                     m_reactor->READ ,
                     boost::bind(&DriverProxy::OnRequestIMP, this, m_sockPair[0]));
                    
 /*   m_reactor->AddFd(STDIN, 
                     m_reactor->READ ,
                     boost::bind(&DriverProxy::ReactorStopTaskIMP, this, STDIN));*/
    //m_thread = boost::thread(boost::bind(&Reactor::Run, m_reactor)); 
    m_thread_nbd = boost::thread(&DriverProxy::NbdConnectionInitializerIMP, 
                                 this, 
                                 m_nbdFd, 
                                 m_sockPair[1]);
    m_thread_run = boost::thread(&Reactor::Run, m_reactor);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
DriverProxy::~DriverProxy()
{
    m_reactor = NULL;
    m_thread_run.join();
    m_thread_nbd.join();
    close(m_sockPair[0]);
    close(m_sockPair[1]);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::ReplyRead(const ReplyPacketRead& packet_)
{
    struct nbd_reply nbdReply;

    CreateReplyPacketReadIMP(&nbdReply, &packet_);
    WriteAllIMP(m_sockPair[0], (char *)&nbdReply, sizeof(nbdReply));
    WriteAllIMP(m_sockPair[0], (char *)&packet_.data, sizeof(packet_.data));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::ReplyWrite(const ReplyPacketWrite& packet_)
{
    struct nbd_reply nbdReply;

    CreateReplyPacketWriteIMP(&nbdReply, &packet_);
    WriteAllIMP(m_sockPair[0], (char *)&nbdReply, sizeof(nbdReply));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateReplyPacketReadIMP(struct nbd_reply *nbdReply, 
								          const ReplyPacketRead *packet_)
{
    nbdReply->magic = htonl(NBD_REPLY_MAGIC);
    nbdReply->error = packet_->status;
    memcpy(&nbdReply->handle, &packet_->uid, sizeof(nbdReply->handle));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateReplyPacketWriteIMP(struct nbd_reply *nbdReply, 
								            const ReplyPacketWrite *packet_)
{
    nbdReply->magic = htonl(NBD_REPLY_MAGIC);
    nbdReply->error = packet_->status;
    memcpy(&nbdReply->handle, &packet_->uid, sizeof(nbdReply->handle));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::OnRequestIMP(int fd_)
{
    size_t bytes_read = 0;
    std::cout << "OnRequestIMP\n";
    struct nbd_request nbdRequest;

    while ((bytes_read = read(fd_, &nbdRequest, sizeof(nbdRequest))) > 0)
    {
        HandleErrorIfExists(true == (nbdRequest.magic == htonl(NBD_REQUEST_MAGIC)) ?
                            0 : -1, "Request magic number is illegal");

        switch (nbdRequest.type)
        {
            case NBD_CMD_WRITE:
            {
                RequestPacketWrite reqWrite;
                CreateRequestPacketWriteIMP(fd_, &reqWrite, &nbdRequest);
                m_onWrite(reqWrite);
                break;
            }
            default:
            {
                RequestPacketRead reqRead;
                CreateRequestPacketReadIMP(&reqRead, &nbdRequest);
                m_onRead(reqRead);
                break;
            }
        }
    }

    HandleErrorIfExists(bytes_read, "error reading user side of nbd socket");
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateRequestPacketReadIMP(RequestPacketRead *packet, 
                                             const nbd_request *nbdPacket)
{
    memcpy(&packet->uid, &nbdPacket->handle, sizeof(uint64_t));
    packet->offset = nbdPacket->from;
    packet->len = nbdPacket->len;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateRequestPacketWriteIMP(int fd, 
                                              RequestPacketWrite *packet,
                                              const nbd_request *nbdPacket)
{
    size_t bytes_read = 0;

    memcpy(&packet->uid, &nbdPacket->handle, sizeof(uint64_t));
    packet->offset = nbdPacket->from;
    packet->len = nbdPacket->len;

    char *buff = new char[nbdPacket->len];
    boost::shared_ptr<char> buff_ptr(buff);

    while ((bytes_read = read(fd, buff_ptr.get(), nbdPacket->len)) > 0)
    {
        InsertDataChunkToReqPacketIMP(&packet->data, bytes_read, buff_ptr.get());
    }
    
    HandleErrorIfExists(bytes_read, "error reading user side of nbd socket");
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::InsertDataChunkToReqPacketIMP(std::vector<char> *src, 
									            size_t len, 
									            char *buff)
{
    for (size_t i = 0; i < len; ++i)
    {
        src->push_back(buff[i]);
    }
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*void DriverProxy::PopDataChunkFromRepPacketIMP(char *buff, 
                                               size_t len, 
                                               std::vector<char> *src)
{
    for (size_t i = 0; i < len; ++i)
    {

        src->push_back(buff[i]);
    }
}*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::ReactorStopTaskIMP(int fd_)
{
    std::cout << "inside ReactorStopTaskIMP\n";
    char buff[10];
    fgets(buff, sizeof(buff), stdin);
    if (0 == strcmp(buff, "stop\n"))
    {
        std::cout << "Inside strcmp\n";
        m_reactor->Stop();
        std::cout << "Inside strcmp after stop\n";
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
}

/******************************************************************************
            Auxilary functions outside of class DriverProxy
******************************************************************************/
void WriteAllIMP(int fd, char *buff, size_t count)
{
    size_t bytes_written = 0;
    while (count > 0)
    {
        bytes_written = write(fd, buff, count);
        HandleErrorIfExists(bytes_written, "Failed to write to buffer");
        buff += bytes_written;
        count -= bytes_written;
    }
}

} // end of namespace ilrd    
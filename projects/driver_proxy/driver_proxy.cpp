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

    HandleErrorIfExists(0 == socketpair(AF_UNIX, SOCK_STREAM, 0, m_sockPair) ? 
                        0 : -1, "create socket pair");
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_SET_BLKSIZE, 4096), 
                        "Failed to set blockSize");
    HandleErrorIfExists(ioctl(m_nbdFd, NBD_SET_SIZE_BLOCKS, 128), 
                        "Failed to set num of blocks");
    m_reactor->AddFd(m_sockPair[0], 
                     m_reactor->READ ,
                     boost::bind(&DriverProxy::OnRequestIMP, this, m_sockPair[0]));
    /* m_reactor->AddFd(m_sockPair[0], 
                     m_reactor->WRITE ,
                     boost::bind(&DriverProxy::OnRequestIMP, this, m_sockPair[0]));                 
      */              
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
    std::cout << "inside reply read\n";

    CreateReplyPacketReadIMP(&nbdReply, &packet_);
    WriteAllIMP(m_sockPair[0], (char *)&nbdReply, sizeof(nbdReply));
    std::cout << "inside reply read vectro size = " << packet_.data.size() << "\n";
    WriteAllIMP(m_sockPair[0], (char *)&packet_.data[0], packet_.data.size());
    std::cout << "After reply read\n";
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::ReplyWrite(const ReplyPacketWrite& packet_)
{
    struct nbd_reply nbdReply;
    std::cout << "inside reply write\n";

    CreateReplyPacketWriteIMP(&nbdReply, &packet_);
    WriteAllIMP(m_sockPair[0], (char *)&nbdReply, sizeof(nbdReply));
    std::cout << "end of ReplyWrite\n";
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateReplyPacketReadIMP(struct nbd_reply *nbdReply, 
								          const ReplyPacketRead *packet_)
{
    std::cout << "inside CreateReplyPacketReadIMP\n";
    nbdReply->magic = htonl(NBD_REPLY_MAGIC);
    nbdReply->error = packet_->status;
    ReverseEndianessIMP2(&nbdReply->error);
    memcpy(&nbdReply->handle, &packet_->uid, sizeof(nbdReply->handle));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateReplyPacketWriteIMP(struct nbd_reply *nbdReply, 
								            const ReplyPacketWrite *packet_)
{
    std::cout << "inside CreateReplyPacketWritreIMP\n";
    nbdReply->magic = htonl(NBD_REPLY_MAGIC);
    nbdReply->error = packet_->status;
    ReverseEndianessIMP2(&nbdReply->error);
    memcpy(&nbdReply->handle, &packet_->uid, sizeof(nbdReply->handle));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::OnRequestIMP(int fd_)
{
    size_t bytes_read = 0;
    std::cout << "OnRequestIMP\n";
    struct nbd_request nbdRequest;

 //   while ((bytes_read = read(fd_, &nbdRequest, sizeof(nbdRequest))) > 0)
   std::cout << "fd = " << fd_ << "\n";
    int moshe = read(fd_, &nbdRequest, sizeof(nbdRequest));
 /*   std::cout << "moshe = " << moshe << "\n";
    if (moshe == -1)
    {
        perror("moshe error");
    }
*/
    {
        std::cout << "bytes_read = " << bytes_read << "\n";
     /*   HandleErrorIfExists(true == (nbdRequest.magic == htonl(NBD_REQUEST_MAGIC)) ?
                            0 : -1, "Request magic number is illegal");
        std::cout << "Inside while of onRequest\n";
        int type = htonl(nbdRequest.type);
        std::cout << "type = " << type << "\n";
        switch (type)
        {
            case NBD_CMD_WRITE:
            {
                std::cout << "Inside NBD_CMD_WRITE\n";
                RequestPacketWrite reqWrite;
                CreateRequestPacketWriteIMP(fd_, &reqWrite, &nbdRequest);
                m_onWrite(reqWrite);
                std::cout << "Inside NBD_CMD_WRITE before bvreak\n";
                break;
            }
            default:
            {
                std::cout << "Inside default\n";
                RequestPacketRead reqRead;
                CreateRequestPacketReadIMP(&reqRead, &nbdRequest);
                m_onRead(reqRead);
                std::cout << "Inside default before break\n";
                break;
            }
        }*/

        
    }
    std::cout << "after while of onRequest\n";
/**/
     HandleErrorIfExists(true == (nbdRequest.magic == htonl(NBD_REQUEST_MAGIC)) ?
                            0 : -1, "Request magic number is illegal");
        std::cout << "Inside while of onRequest\n";
        int type = htonl(nbdRequest.type);
        std::cout << "type = " << type << "\n";
        switch (type)
        {
            case NBD_CMD_WRITE:
            {
                std::cout << "Inside NBD_CMD_WRITE\n";
                RequestPacketWrite reqWrite;
                CreateRequestPacketWriteIMP(fd_, &reqWrite, &nbdRequest);
                m_onWrite(reqWrite);
                std::cout << "Inside NBD_CMD_WRITE before bvreak\n";
                break;
            }
            case NBD_CMD_READ:
            {
                std::cout << "Inside NBD_CMD_READ\n";
                RequestPacketRead reqRead;
                CreateRequestPacketReadIMP(&reqRead, &nbdRequest);
                m_onRead(reqRead);
                std::cout << "Inside default before break\n";
                break;
            }
        }
        std::cout << "outside switch case\n";
  /**/      

    HandleErrorIfExists(bytes_read, "error reading user side of nbd socket");
    std::cout << "afterhandlke errorrsz\n";
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateRequestPacketReadIMP(RequestPacketRead *packet, 
                                             const nbd_request *nbdPacket)
{
    memcpy(&packet->uid, &nbdPacket->handle, sizeof(uint64_t));
    packet->offset = nbdPacket->from; // need to convert from to offset char char
    ReverseEndianessIMP1(&packet->offset);
    packet->len = htonl(nbdPacket->len);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::CreateRequestPacketWriteIMP(int fd, 
                                              RequestPacketWrite *packet,
                                              const nbd_request *nbdPacket)
{
    std::cout << "Inside CreateRequestPacketWriteIMP\n";
    size_t bytes_read = 0;

    memcpy(&packet->uid, &nbdPacket->handle, sizeof(uint64_t));
    packet->offset = nbdPacket->from; // need to convert from to offset char char
    ReverseEndianessIMP1(&packet->offset);
    packet->len = htonl(nbdPacket->len);

    char *buff = new char[nbdPacket->len];
    boost::shared_ptr<char> buff_ptr(buff);

    //while ((bytes_read = read(fd, buff_ptr.get(), packet->len)) > 0)
    read(fd, buff_ptr.get(), packet->len);
    {
        InsertDataChunkToReqPacketIMP(&packet->data, bytes_read, buff_ptr.get());
    }
    
    HandleErrorIfExists(bytes_read, "error reading user side of nbd socket");
    std::cout << "end f CreateRequestPacketWriteIMP\n";
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DriverProxy::InsertDataChunkToReqPacketIMP(std::vector<char> *src, 
									            size_t len, 
									            char *buff)
{
    std::cout << "Inside InsertDataChunkToReqPacketIMP\n";
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
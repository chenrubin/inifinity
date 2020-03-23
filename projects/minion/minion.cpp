
#include <arpa/inet.h> /* send, connect */
#include <boost/bind.hpp> /* boost::bind */
#include <stdio.h> /* perror */
#include <iostream> /* cout */
#include <algorithm> /* reverse */ 

#include "minion.hpp"

#define DATA_OFFSET (17)
#define DATA_LENGTH (4 * 1024)
#define READ_RESPONSE_LENGTH ((10) + (DATA_LENGTH))
#define WRITE_RESPONSE_LENGTH (10)
#define IS_LITLLE_ENDIAN (1 == (*(int *)(&"\1\0\0\0"))? 1:0)
#define SIZEOF_UID 8
#define SIZEOF_BLOCKINDEX 8


namespace ilrd
{
namespace
{
void ParseMessageIMP(u_int64_t *uid, 
                     u_int64_t *blockIndex, 
                     unsigned char *type, 
                     char *buff);
}

Minion::Minion(unsigned short port_)
    : m_socket(port_, INADDR_LOOPBACK, SO_REUSEADDR, false)
    , m_reactor()
    , m_storage()
{
    m_storage = new Storage;
    m_reactor.AddFd(m_socket.GetFd(), m_reactor.READ, boost::bind(Callback, this));
}

Minion::~Minion()
{
    delete(m_storage);
}

void Minion::Run()
{
    m_reactor.Run();
}

void Minion::Stop()
{
    m_reactor.Stop();
}

void Minion::RecvRequestIMP(int fd_)
{
    char read_buff[BLOCK_SIZE];
    struct sockaddr_in addr;
    socklen_t size = sizeof(addr);
    ssize_t bytes = recvfrom(fd_, read_buff, BLOCK_SIZE, 0, (struct sockaddr *)&addr, &size);

    /*size_t*/u_int64_t uid = 0;
    /*size_t*/u_int64_t blockIndex = 0;
    unsigned char type = 0;

    if (0 < bytes)
    {
        ParseMessageIMP(&uid, &blockIndex, &type, read_buff);
        HandleRequestIMP(uid, blockIndex, type, read_buff);
        SendResponseIMP(type, uid, read_buff, &addr);
    }
    else if (0 == bytes)
    {
        m_reactor.RemoveFd(fd_, m_reactor.READ);
        throw std::runtime_error("socket closed");
    }
    else
    {
        perror("server recvfrom");
        throw std::runtime_error("recvfrom failed");
    }
    
}

void Minion::HandleRequestIMP(/*size_t*/u_int64_t uid, 
                              /*size_t*/u_int64_t blockIndex, 
                              unsigned char type, 
                              char *buff)
{
    if (0 == type) // read from storage into buffer
    {
        m_storage->Read(blockIndex, buff + DATA_OFFSET);
    }
    else // write to storage
    {
        m_storage->Write(blockIndex, buff + DATA_OFFSET);
    }
}

void Minion::SendResponseIMP(unsigned char type, 
						     /*size_t*/u_int64_t uid,
						     char *databuff, 
						     struct sockaddr_in *addr)
{
    char buf_to_send[BLOCK_SIZE];
    BuildBuffIMP(type, uid ,databuff, buf_to_send);
    size_t len = (1 == type) ? WRITE_RESPONSE_LENGTH : READ_RESPONSE_LENGTH; 

    if (IS_LITLLE_ENDIAN)
    {
        std::reverse(buf_to_send + 1, buf_to_send + 9);
    }

    if (-1 == sendto(m_socket.GetFd(), buf_to_send, 
                     len, MSG_CONFIRM,
                     (struct sockaddr *)addr, 
                     sizeof(*addr)))
    {
        perror("server sendto");
        throw std::runtime_error("sendto failed");
    }
}

void Minion::BuildBuffIMP(unsigned char type, 
			              /*size_t*/u_int64_t uid,
			              char *databuff,
                          char *buffToBuild)
{
    bool status = true;

    buffToBuild[0] = type;
    *(size_t *)(buffToBuild + 1) = uid;
    buffToBuild[9] = status;
    if (0 == type)
    {
        memcpy(buffToBuild + 10, databuff + DATA_OFFSET, DATA_LENGTH);
    }
}

void Minion::Callback(Minion *minion)
{
    minion->RecvRequestIMP(minion->m_socket.GetFd());
}

namespace
{
void ParseMessageIMP(/*size_t*/u_int64_t *uid, 
                     /*size_t*/u_int64_t *blockIndex, 
                     unsigned char *type, 
                     char *buff)
{
    if (IS_LITLLE_ENDIAN)
    {
        std::reverse(buff + 1, buff + 9);
        std::reverse(buff + 9, buff + 17);
    }

    *type = *buff;
    *uid = *((u_int64_t *)(buff + 1));
    *blockIndex = *((u_int64_t *)(buff + 9));
}

} // end of namespace
    


} // end of ilrd nemaspace
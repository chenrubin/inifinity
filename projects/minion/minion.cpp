
#include <arpa/inet.h> /* send, connect */
#include <boost/bind.hpp> /* boost::bind */
#include <stdio.h> /* perror */
#include <iostream> /* cout */
#include <algorithm> /* reverse */ 

#include <fcntl.h>      // open , O_CREAT, O_APPEND, O_RDWR


#include "minion.hpp"

#define DATA_OFFSET (17)
#define DATA_LENGTH (4 * 1024)
#define NUM_OF_BLOCKS (64)
#define READ_RESPONSE_LENGTH ((10) + (DATA_LENGTH))
#define WRITE_RESPONSE_LENGTH (10)
#define IS_LITLLE_ENDIAN (1 == (*(int *)(&"\1\0\0\0"))? 1:0)
#define SIZEOF_UID 8
#define SIZEOF_BLOCKINDEX 8
#define STDIN (0)


namespace ilrd
{
Minion::Minion(unsigned short port_)
    : m_socket(port_, /*"192.168.1.20"*/"127.0.0.1", true)
    , m_reactor()
    , m_storageFd(TruncateStorageIMP())
{
//    m_storage = new Storage;
    m_reactor.AddFd(m_socket.GetFd(), m_reactor.READ, boost::bind(Callback, this));
    m_reactor.AddFd(STDIN, m_reactor.READ, boost::bind(&Minion::StopMinionCallbackIMP, this, _1));
}

Minion::~Minion()
{
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!inside Minion Dtor\n";
    if (-1 == close(m_storageFd))
    {
        perror("Close failed");
    }
    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!inside Minion Dtor after close\n";
 //   delete(m_storage);
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
    char *read_buff = new char[/*m_storage->BLOCK_SIZE*/DATA_LENGTH + DATA_OFFSET];
    struct sockaddr_in addr;
    socklen_t size = sizeof(addr);
    ssize_t bytes = recvfrom(fd_, read_buff, /*m_storage->BLOCK_SIZE*/DATA_LENGTH + DATA_OFFSET, 0, (struct sockaddr *)&addr, &size);
    std::cout << "\n!!!!!read_buffer = " << read_buff << "!!!!!\n";
    u_int64_t uid = 0;
    u_int64_t blockIndex = 0;
    unsigned char type = 0;
    std::cout << "inside receive request\n";
    if (0 < bytes)
    {
        std::cout << "Starting parse, handle and send response\n";
        LOG_DEBUG("Starting parse, handle and send response");
        ParseMessageIMP(&uid, &blockIndex, &type, read_buff);
        HandleRequestIMP(uid, blockIndex, type, read_buff);
        SendResponseIMP(type, uid, read_buff, &addr);
    }
    else if (0 == bytes)
    {
        m_reactor.RemoveFd(fd_, m_reactor.READ);
        throw std::runtime_error("socket closed");
        LOG_ERROR("socket closed");    ;
    }
    else
    {
        perror("server recvfrom");
        throw std::runtime_error("recvfrom failed");
        //LOG_ERROR("server rcvfrom failed");    
    }
    // why delete result in double free?//(read(m_storageFd, buff + DATA_OFFSET, DATA_LENGTH));
    // where else do I delete this buffer?
    delete[] read_buff;
}

void Minion::HandleRequestIMP(u_int64_t uid, 
                              u_int64_t blockIndex, 
                              unsigned char type, 
                              char *buff)
{
    if (0 == type) // read from storage into buffer
    {
        std::cout << "!!!Read from storage!!!\n";
        std::cout << "!!!blockIndex = " << blockIndex << std::endl;
        std::cout << "!!!buff + DATA_OFFSET = " << buff + DATA_OFFSET << std::endl;
        //std:cout << "DATA_OFFSET"
        std::cout << "blockIndex = " << blockIndex << "\n";
        //m_storage->Read(blockIndex, buff + DATA_OFFSET);
        HandleErrorIfExists(read(m_storageFd, buff + DATA_OFFSET, DATA_LENGTH),
                                 "HandleRequestIMP failed to read");

        
        LOG_DEBUG("Read from storage");
    }
    else // write to storage
    {
        //m_storage->Write(blockIndex, buff + DATA_OFFSET);
        HandleErrorIfExists(write(m_storageFd, buff + DATA_OFFSET, DATA_LENGTH),
                                  "HandleRequestIMP failed to write");
        std::cout << "Write to storage\n";
        LOG_DEBUG("Write to storage");
    }
}

void Minion::SendResponseIMP(unsigned char type, 
						     u_int64_t uid,
						     char *databuff, 
						     struct sockaddr_in *addr)
{
    std::cout << "Inside SendResponseIMP\n";
    char buf_to_send[/*m_storage->BLOCK_SIZE + 10*/READ_RESPONSE_LENGTH];
    BuildBuffIMP(type, uid ,databuff, buf_to_send);
    std::cout << "After building response buffer\n";
    size_t len = (1 == type) ? WRITE_RESPONSE_LENGTH : READ_RESPONSE_LENGTH; 

    if (IS_LITLLE_ENDIAN)
    {
        std::cout << "inside if (IS_LITLLE_ENDIAN)\n";
        std::reverse(buf_to_send + 1, buf_to_send + 9);
    }
    std::cout << "About to send message\n";
    LOG_DEBUG("About to send message");
    
    if (-1 == sendto(m_socket.GetFd(), buf_to_send, 
                     len, MSG_CONFIRM,
                     (struct sockaddr *)addr, 
                     sizeof(*addr)))
    {
        perror("server sendto");
        throw std::runtime_error("sendto failed");
        LOG_ERROR("sendto failed");
    }
}

void Minion::BuildBuffIMP(unsigned char type, 
			              u_int64_t uid,
			              char *databuff,
                          char *buffToBuild)
{
    bool status = /*true*/false;
    std::cout << "Building response buffer\n";
    LOG_DEBUG("Building response buffer");

    buffToBuild[0] = type;
    *(uint64_t *)(buffToBuild + 1) = uid;
    std::cout << "Inside BuildBuffIMP uid = " << uid << "\n";
    buffToBuild[9] = status;
    if (0 == type)
    {
        memcpy(buffToBuild + 10, databuff + DATA_OFFSET, DATA_LENGTH);
    }
    std::cout << "end of Building response buffer\n";
}

void Minion::Callback(Minion *minion)
{
    std::cout << "About to invoke minion->RecvRequestIMP\n";
    minion->RecvRequestIMP(minion->m_socket.GetFd());
}

void Minion::ParseMessageIMP(u_int64_t *uid, 
                     u_int64_t *blockIndex, 
                     unsigned char *type, 
                     char *buff)
{
    std::cout << "Inside Parse message\n";
    printf("!!!buffer = %s\n!!!", buff);
    //std::cout << "!!buffer = " << buff << "!!\n";
    if (IS_LITLLE_ENDIAN)
    {
        std::reverse(buff + 1, buff + 9);
        std::reverse(buff + 9, buff + 17);
    }

    *type = *buff;
    *uid = *((u_int64_t *)(buff + 1));
    *blockIndex = *((u_int64_t *)(buff + 9));
    std::cout << "Inside parse message blockIndex = " << *blockIndex << "\n";
    LOG_DEBUG("Parse message");
}

int Minion::TruncateStorageIMP()
{
    m_storageFd = open("/home/chenr/storage_minion1.txt", O_CREAT | O_RDWR, 0777);
    if (-1 == m_storageFd)
    {
        std::cout << "error = " << strerror(errno) << "\n";
    }
    
    HandleErrorIfExists(m_storageFd, "TruncateStorageIMP - failed to open");
    HandleErrorIfExists(ftruncate(m_storageFd, DATA_LENGTH * NUM_OF_BLOCKS), 
                        "TruncateStorageIMP - failed to truncate fd");


    return m_storageFd;
}

void Minion::StopMinionCallbackIMP(int fd_)
{
    char buff[20];
    std::fgets(buff, sizeof(buff), stdin);
    if (0 == strcmp(buff, "stop\n"))
    {
        //LOG_DEBUG("inside stop");
       Stop();
    }
}    
} // end of ilrd nemaspace
#include <iostream>                 // std::cout
#include <stdio.h>                  // perror
#include <string.h>                 // memcpy
#include <algorithm>                // std::reverse
#include <unistd.h>                 // write

#include "factoryTask.hpp"          // class Task
#include "../../cpp_fs/utils/MyUtils.hpp"	// HandleErrors

#define DATA_OFFSET (17)
#define DATA_LENGTH (4 * 1024)
#define WRITE_RESPONSE_LENGTH (10)
#define READ_RESPONSE_LENGTH ((10) + (DATA_LENGTH))
#define IS_LITLLE_ENDIAN (1 == (*(int *)(&"\1\0\0\0"))? 1:0)

namespace ilrd
{
/*-----------------------------------------------------------------------------
                                Task functions
-----------------------------------------------------------------------------*/
Task::Task()
{}

Task::~Task()
{}

/*-----------------------------------------------------------------------------
                                WriteTask functions
-----------------------------------------------------------------------------*/
WriteTask::WriteTask()
    : m_dataOffset(DATA_OFFSET)
    , m_dataLength(DATA_LENGTH)
    , m_responseLength(WRITE_RESPONSE_LENGTH)
{}

WriteTask::~WriteTask()
{}

void WriteTask::ImplementTask(int fd_, char *buff, sockaddr_in *addr, int minionFd)
{
    //HandleRequestIMP
    HandleErrorIfExists(write(fd_, buff + m_dataOffset, m_dataLength),
                                  "HandleRequestIMP failed to write");
    std::cout << "Write to storage\n";  
    char buf_to_send[WRITE_RESPONSE_LENGTH];                              
    BuildResponseBuffIMP(buff, buf_to_send);

    if (IS_LITLLE_ENDIAN)
    {
        std::cout << "inside if (IS_LITLLE_ENDIAN)\n";
        std::reverse(buf_to_send + 1, buf_to_send + 9);
    }

    SendResponseIMP(buf_to_send, addr, minionFd);
}

void WriteTask::BuildResponseBuffIMP(char *buff, char *buf_to_send)
{
    bool status = false;
    std::cout << "Building response buffer\n";
//    LOG_DEBUG("Building response buffer");

    buf_to_send[0] = buff[0];
    *(uint64_t *)(buf_to_send + 1) = *(uint64_t *)(buff + 1);

    std::cout << "end of Building response buffer\n";
}

void WriteTask::SendResponseIMP(char *read_buff, sockaddr_in *addr, int fdToSendTo)
{
    if (-1 == sendto(fdToSendTo, read_buff, 
                     m_responseLength, MSG_CONFIRM,
                     (struct sockaddr *)addr,
                     sizeof(*addr)))
    {
        perror("server sendto");
        throw std::runtime_error("sendto failed");
 //       LOG_ERROR("sendto failed");
    }
}

/*-----------------------------------------------------------------------------
                                ReadTask functions
-----------------------------------------------------------------------------*/
ReadTask::ReadTask()
    : m_dataOffset(DATA_OFFSET)
    , m_dataLength(DATA_LENGTH)
    , m_responseLength(READ_RESPONSE_LENGTH)
{}

ReadTask::~ReadTask()
{}

void ReadTask::ImplementTask(int fd_, char *buff, sockaddr_in *addr, int minionFd)
{
    //HandleRequestIMP
    HandleErrorIfExists(read(fd_, buff + m_dataOffset, m_dataLength),
                                 "HandleRequestIMP failed to read");
    std::cout << "read from storage\n";  
    char buf_to_send[READ_RESPONSE_LENGTH];                              
    BuildResponseBuffIMP(buff, buf_to_send);

    if (IS_LITLLE_ENDIAN)
    {
        std::cout << "inside if (IS_LITLLE_ENDIAN)\n";
        std::reverse(buf_to_send + 1, buf_to_send + 9);
    }

    SendResponseIMP(buf_to_send, addr, minionFd);
}

void ReadTask::BuildResponseBuffIMP(char *buff, char *buf_to_send)
{
    bool status = false;
    std::cout << "Building response buffer\n";
//    LOG_DEBUG("Building response buffer");

    buf_to_send[0] = buff[0]; // ?
    *(uint64_t *)(buf_to_send + 1) = *(uint64_t *)(buff + 1); // ?
    memcpy(buf_to_send + 10, buff + m_dataOffset, m_dataLength);

    std::cout << "end of Building response buffer\n";
}

void ReadTask::SendResponseIMP(char *read_buff, sockaddr_in *addr, int fdToSendTo)
{
    if (-1 == sendto(fdToSendTo, read_buff, 
                     m_responseLength, MSG_CONFIRM,
                     (struct sockaddr *)addr,
                     sizeof(*addr)))
    {
        perror("server sendto");
        throw std::runtime_error("sendto failed");
//        LOG_ERROR("sendto failed");
    }
}

} // namespace ilrd

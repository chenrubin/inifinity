#include "factoryTask.hpp"          // class Task

#define DATA_OFFSET (17)
#define DATA_LENGTH (4 * 1024)
#define WRITE_RESPONSE_LENGTH (10)
#define READ_RESPONSE_LENGTH ((10) + (DATA_LENGTH))

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
WriteTask::WriteTask(size_t dataOffset_, size_t dataLength_, 
                                         size_t responseLength_)
    : m_dataOffset(dataOffset_)
    , m_dataLength(dataLength_)
    , m_responseLength(responseLength_)
{}

WriteTask::~WriteTask()
{}

void WriteTask::ImplementTask(int fd_, char *buff, sockaddr_in *addr, int minionFd)
{
    //HandleRequestIMP
    HandleErrorIfExists(write(fd_, buff + m_dataOffset, m_dataLength),
                                  "HandleRequestIMP failed to write");
    std::cout << "Write to storage\n";  
    char buf_to_send[m_responseLength];                              
    BuildResponseBuffIMP(buff, buf_to_send);

    if (IS_LITLLE_ENDIAN)
    {
        std::cout << "inside if (IS_LITLLE_ENDIAN)\n";
        std::reverse(buf_to_send + 1, buf_to_send + 9);
    }

    SendResponseIMP(buff_to_send, addr, minionFd);
}

void WriteTask::BuildResponseBuffIMP(char *buff, char *buf_to_send)
{
    bool status = false;
    std::cout << "Building response buffer\n";
    LOG_DEBUG("Building response buffer");

    buf_to_send[0] = type;
    *(uint64_t *)(buf_to_send + 1) = uid;

    std::cout << "end of Building response buffer\n";
}

void WriteTask::SendResponseIMP(char *read_buff, sockaddr_in *addr, int fdToSendTo)
{
    if (-1 == sendto(fdToSendTo, buf_to_send, 
                     WRITE_RESPONSE_LENGTH, MSG_CONFIRM,
                     (struct sockaddr *)addr,
                     sizeof(*addr)))
    {
        perror("server sendto");
        throw std::runtime_error("sendto failed");
        LOG_ERROR("sendto failed");
    }
}

/*-----------------------------------------------------------------------------
                                ReadTask functions
-----------------------------------------------------------------------------*/
ReadTask::ReadTask(size_t dataOffset_, size_t dataLength_, 
                                       size_t responseLength_)
    : m_dataOffset(dataOffset_)
    , m_dataLength(dataLength_)
    , m_responseLength(responseLength_)                                       
{}

void ReadTask::BuildResponseBuffIMP(char *buff, char *buf_to_send)
{
    bool status = false;
    std::cout << "Building response buffer\n";
    LOG_DEBUG("Building response buffer");

    buf_to_send[0] = type;
    *(uint64_t *)(buf_to_send + 1) = uid;
    memcpy(buf_to_send + 10, databuff + DATA_OFFSET, DATA_LENGTH);

    std::cout << "end of Building response buffer\n";
}

void ReadTask::SendResponseIMP(char *read_buff, sockaddr_in *addr, int fdToSendTo)
{
    if (-1 == sendto(fdToSendTo, buf_to_send, 
                     READ_RESPONSE_LENGTH, MSG_CONFIRM,
                     (struct sockaddr *)addr,
                     sizeof(*addr)))
    {
        perror("server sendto");
        throw std::runtime_error("sendto failed");
        LOG_ERROR("sendto failed");
    }
}

} // namespace ilrd

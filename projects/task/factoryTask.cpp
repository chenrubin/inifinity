#include "factoryTask.hpp"          // class Task

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

void WriteTask::ImplementTask(int fd_, char *buff, sockaddr_in *addr)
{
    //HandleRequestIMP
    HandleErrorIfExists(write(fd_, buff + m_dataOffset, m_dataLength),
                                  "HandleRequestIMP failed to write");
    std::cout << "Write to storage\n";  
    char buf_to_send[m_responseLength];                              
    BuildResponseBuffIMP(buff, buf_to_send);
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

} // namespace ilrd

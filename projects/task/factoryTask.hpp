/*----------------------------------------------------------------------------*/
#ifndef __FACTORY_TASK_HPP__ 
#define __FACTORY_TASK_HPP__
/*----------------------------------------------------------------------------*/
#include <stddef.h>             // size_t
#include <netinet/in.h>         // sockaddr_in
/*----------------------------------------------------------------------------*/
namespace ilrd
{
class Task 
{
public:
    explicit Task();
    virtual ~Task();
/*    virtual size_t GetdataOffset() = 0;
    virtual size_t GetdataLength() = 0;
    virtual size_t GetresponseLength() = 0;
*/
    virtual void ImplementTask(int fd_, char *buff, sockaddr_in *addr, int minionFd) = 0;
private:
};

class WriteTask : public Task
{
public:
    explicit WriteTask();
    virtual ~WriteTask();

    virtual void ImplementTask(int fd_, char *buff, sockaddr_in *addr, int minionFd);
private:
    void BuildResponseBuffIMP(char *buff, char *buf_to_send);
    void SendResponseIMP(char *read_buff, sockaddr_in *addr, int fdToSendTo);

    size_t m_dataOffset;
    size_t m_dataLength;
    size_t m_responseLength;
};

class ReadTask : public Task
{
public:
    explicit ReadTask();
    virtual ~ReadTask();

    virtual void ImplementTask(int fd_, char *buff, sockaddr_in *addr, int minionFd);
private:
    void BuildResponseBuffIMP(char *buff, char *buf_to_send);
    void SendResponseIMP(char *read_buff, sockaddr_in * addr, int fdToSendTo);

    size_t m_dataOffset;
    size_t m_dataLength;
    size_t m_responseLength;
};
}
#endif
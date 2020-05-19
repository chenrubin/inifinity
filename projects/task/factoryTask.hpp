/*----------------------------------------------------------------------------*/
#ifndef __FACTORY_TASK_HPP__ 
#define __FACTORY_TASK_HPP__
/*----------------------------------------------------------------------------*/
#include <stddef.h>             // size_t
/*----------------------------------------------------------------------------*/
namespace ilrd
{
class Task 
{
public:
    explicit Task();
    virtual ~Task();
    size_t GetdataOffset();
    size_t GetdataLength();
    size_t GetresponseLength();

    virtual void ImplementTask() = 0;
private:
};

class WriteTask : public Task
{
public:
    explicit WriteTask(size_t dataOffset_, size_t dataLength_, 
                                           size_t responseLength_);
    virtual ~WriteTask();

    virtual void ImplementTask(int fd_, char *buff, sockaddr_in *addr);
private:
    BuildResponseBuffIMP(char *buff, char *buf_to_send);
    SendResponseIMP(char *read_buff, &addr);

    size_t m_dataOffset;
    size_t m_dataLength;
    size_t m_responseLength;
};

class ReadTask : public Task
{
public:
    explicit ReadTask(size_t dataOffset_, size_t dataLength_, 
                                          size_t responseLength_);
    virtual ~ReadTask();

    virtual void ImplementTask(int fd_, char *buff);
private:
    BuildResponseBuffIMP(buff, buf_to_send);

    size_t m_dataOffset;
    size_t m_dataLength;
    size_t m_responseLength;
};

struct MinionParams
{
    size_t dataOffset;
    size_t dataLength;
    size_t responseLength;
    //DATA_OFFSET, DATA_LENGTH, RESPONSE_LENGTH
};

}
#endif
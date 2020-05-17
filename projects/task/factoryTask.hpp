/*----------------------------------------------------------------------------*/
#ifndef __FACTORY_TASK_HPP__ 
#define __FACTORY_TASK_HPP__
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
namespace ilrd
{
class Task 
{
public:
    explicit Task();
    virtual ~Task();

    virtual ImplementTask() = 0;
private:
};

class WriteTask : public Task
{
public:
    explicit WriteTask();
    virtual ~WriteTask();

    virtual ImplementTask(int fd_, char *buff);
};

class ReadTask : public Task
{
public:
    explicit ReadTask();
    virtual ~ReadTask();

    virtual ImplementTask(int fd_, char *buff);
};

struct MinionParams
{
    size_t dataOffset;
    size_t dataLength;
    size_t responseLength;
    //DATA_OFFSET, DATA_LENGTH, RESPONSE_LENGTH
}

}
#endif
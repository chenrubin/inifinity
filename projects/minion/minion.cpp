
#include <arpa/inet.h> /* send, connect */
#include <boost/bind.hpp> /* boost::bind */
#include <stdio.h> /* perror */
#include <iostream> /* cout */
#include <algorithm> /* reverse */
#include <fstream>

#include "minion.hpp"

#define DATA_OFFSET (17)
#define DATA_LENGTH (4 * 1024)
#define READ_RESPONSE_LENGTH ((10) + (DATA_LENGTH))
#define WRITE_RESPONSE_LENGTH (10)
#define IS_LITLLE_ENDIAN (1 == (*(int *)(&"\1\0\0\0"))? 1:0) 

namespace ilrd
{
namespace
{
void ParseMessageIMP(size_t *uid, 
                        size_t *blockIndex, 
                        unsigned char *type, 
                        char *buff);

bool IsLittleEndian();                        
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
    size_t uid = 0;
    size_t blockIndex = 0;
    unsigned char type = 0;
    std::cout << "inside Minion::RecvRequestIMP before recvfrom\n";
    ssize_t bytes = recvfrom(fd_, read_buff, BLOCK_SIZE, 0, (struct sockaddr *)&addr, &size);
    std::cout << "inside Minion::RecvRequestIMP after recvfrom, byres = "<< bytes << "\n";
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

void Minion::HandleRequestIMP(size_t uid, 
                              size_t blockIndex, 
                              unsigned char type, 
                              char *buff)
{
    std::cout << "inside HandleRequestIMP\n";
    if (0 == type) // read from storage into buffer
    {
        std::cout << "inside HandleRequestIMP -> read\n";
        m_storage->Read(blockIndex, buff + DATA_OFFSET);
    }
    else // write to storage
    {
        std::cout << "inside HandleRequestIMP -> write\n";
        m_storage->Write(blockIndex, buff
         + DATA_OFFSET);
    }
}

void Minion::SendResponseIMP(unsigned char type, 
						     size_t uid,
						     char *databuff, 
						     struct sockaddr_in *addr)
{
    char buf_to_send[BLOCK_SIZE];
    std::cout << "inside sendresponse\n";
    BuildBuffIMP(type, uid ,databuff, buf_to_send);
    size_t len = (1 == type) ? WRITE_RESPONSE_LENGTH : READ_RESPONSE_LENGTH; 
    char str[12] = {0};
    strcpy(str, buf_to_send);
    std::cout << str << "\n";

    if (IS_LITLLE_ENDIAN)
    {
        std::reverse(buf_to_send + 1, buf_to_send + 8);
    }

    std::ofstream myfile("test.txt", std::ofstream::out);
  //   myfile.open ("test.txt");
     std::cout << buf_to_send << "\n";
    myfile <<  buf_to_send;
    myfile.close();
    if (-1 == sendto(m_socket.GetFd(), buf_to_send, 
                     len, MSG_CONFIRM,
                     (struct sockaddr *)addr, 
                     sizeof(*addr)))
    {
        perror("server sendto");
        throw std::runtime_error("sendto failed");
    }
    std::cout << "inside sendresponse after sendto\n";
}

void Minion::BuildBuffIMP(unsigned char type, 
			              size_t uid,
			              char *databuff,
                          char *buffToBuild)
{
    bool status = true;

    buffToBuild[0] = type;
    *(size_t *)(buffToBuild + 1) = uid;
    buffToBuild[9] = status;
    if (0 == type)
    {
        memcpy(buffToBuild + 9, databuff + DATA_OFFSET, DATA_LENGTH);
    }
}

void Minion::Callback(Minion *minion)
{
    std::cout << "inside Minion::Callback\n";
    minion->RecvRequestIMP(minion->m_socket.GetFd());
}

namespace
{
void ParseMessageIMP(size_t *uid, 
                     size_t *blockIndex, 
                     unsigned char *type, 
                     char *buff)
{
    std::cout << "inside ParseMessageIMP\n";

    if (IS_LITLLE_ENDIAN)
    {
        std::reverse(buff + 1, buff + 8);
        std::reverse(buff + 9, buff + 16);
    }

    *type = *buff;
    *uid = *((size_t *)(buff + 1));
    *blockIndex = *((size_t *)(buff + 9));
}

bool IsLittleEndian()  
{  
    unsigned int i = 1;  
    char *c = (char*)&i;  
    char m = *c;
    std::cout << "inside IsLittleEndian *c = " << m << "\n";

    return (*c == 1); 
}  
}
    


}
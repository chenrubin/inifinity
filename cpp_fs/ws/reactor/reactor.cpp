#include <iostream> /* cerr */
#include <cerrno> /* errno */
#include <stdexcept> /* std::runtime_error*/
#include <stdio.h> /* perror */

#include "reactor.hpp" /* reactor Apis */


namespace ilrd
{

namespace
{
    class RemoveFromVector
    { 
    public:      
        RemoveFromVector(int fd_);
        ~RemoveFromVector();

        bool operator()(std::pair<int, boost::function<void(int)> > p);

    private:
        int m_fd;
    };
}

Reactor::Reactor()
    : m_stop(false)
{}

Reactor::~Reactor()
{}

void Reactor::AddFd(int fd_, type_t type_, boost::function<void(int)> callback_)
{
    std::pair<int, boost::function<void(int)> > p(fd_, callback_);
    type_vec[type_].push_back(p);
}

void Reactor::RemoveFd(int fd_, type_t type_)
{
    std::vector<std::pair<int, boost::function<void(int)> > >::iterator iter;
    iter =  std::find_if(type_vec[type_].begin(), 
                         type_vec[type_].end(), 
                         RemoveFromVector(fd_));
    type_vec[type_].erase(iter);                    
}

Reactor::error_t Reactor::Run()
{
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    int maxSockId = 0;

    while (!m_stop)
    {
        maxSockId = GetMaxSocketIMP();
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        UpdateFdSetsIMP(&readfds, &writefds, &exceptfds);
       
        if (-1 == select(maxSockId + 1, &readfds, &writefds, &exceptfds, NULL))
        {
            SelectHandlerIMP(errno);
        }
    
        for (int sockNum = 0; sockNum < maxSockId + 1; ++sockNum)
        {
            if (FD_ISSET(sockNum, &readfds))
            {
                InvokeHandlerIMP(READ, sockNum);
            }
            if (FD_ISSET(sockNum, &writefds))
            {
                InvokeHandlerIMP(WRITE, sockNum);
            }
            if (FD_ISSET(sockNum, &exceptfds))
            {
                InvokeHandlerIMP(EXCEPT, sockNum);
            }
        }
    }
   
    return SUCCESS;
}

void Reactor::Stop()
{
    m_stop = 1;
}

void Reactor::InvokeHandlerIMP(Reactor::type_t type, int sockFd)
{
    std::vector<std::pair<int, boost::function<void(int)> > >::iterator it;

    it = std::find_if(type_vec[type].begin(), 
                      type_vec[type].end(), 
                      RemoveFromVector(sockFd));
    it->second(sockFd);
}

void Reactor::UpdateFdSetsIMP(fd_set *readfds, fd_set *writefds, fd_set *exceptfds)
{
    std::vector<std::pair<int, boost::function<void(int)> > >::iterator it;
    
    for (it = type_vec[READ].begin(); it != type_vec[READ].end(); ++it)
    {
        FD_SET(it->first, readfds);
    }
    for (it = type_vec[WRITE].begin(); it != type_vec[WRITE].end(); ++it)
    {
        FD_SET(it->first, writefds);
    }
    for (it = type_vec[EXCEPT].begin(); it != type_vec[EXCEPT].end(); ++it)
    {
        FD_SET(it->first, exceptfds);
    }
}

int Reactor::GetMaxSocketIMP()
{
    int maxSocket = 0;
    std::vector<std::pair<int, boost::function<void(int)> > >::iterator it;

    for (it = type_vec[READ].begin(); it != type_vec[READ].end(); ++it)
    {
        if (maxSocket < it->first)
        {
            maxSocket = it->first;
        }
    }
    for (it = type_vec[WRITE].begin(); it != type_vec[WRITE].end(); ++it)
    {
        if (maxSocket < it->first)
        {
            maxSocket = it->first;
        }
    }
    for (it = type_vec[EXCEPT].begin(); it != type_vec[EXCEPT].end(); ++it)
    {
        if (maxSocket < it->first)
        {
            maxSocket = it->first;
        }
    }

    return maxSocket;
}

Reactor::error_t Reactor::SelectHandlerIMP(int err)
{
    switch (err)
    {
        case EBADF:
            return EBADF_T;
            break;

        case EINTR:
            return EINTR_T;
            break;

        case EINVAL:
            return EINVAL_T;
            break;

        default:
            return ENOMEM_T;
            break;
    }
}

namespace
{
    RemoveFromVector::RemoveFromVector(int fd_)
        : m_fd(fd_)
    {}

    RemoveFromVector::~RemoveFromVector()
    {}

    bool RemoveFromVector::operator()(std::pair<int, boost::function<void(int)> > p)
    {
        return (m_fd == p.first);
    }
} // end of namespace
} // end of namespace ilrd
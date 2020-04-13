#include <iostream> /* cerr */
#include <cerrno> /* errno */
#include <stdexcept> /* std::runtime_error*/
#include <stdio.h> /* perror */
#include <algorithm> /* transform */

#include "reactor.hpp" /* reactor Apis */


namespace ilrd
{

namespace
{
    void DummyCallback1IMP(int fd);
    class FdCompare
    { 
    public:      
        FdCompare(int fd_);
        ~FdCompare();

        bool operator()(std::pair<int, boost::function<void(int)> > p);

    private:
        int m_fd;
    };

    class FdSet
    {
    public:
        FdSet(fd_set *set_)
            : m_set(set_)
        {}

        void operator ()(std::pair<int, boost::function<void(int)> > pr);  
    
    private:
        fd_set *m_set;
    };

    class MaxSocket
    {
    public:
        MaxSocket(int *num)
            : m_num(num)
        {}

        void operator()(std::pair<int, boost::function<void(int)> > pr);
    
    private:
        int *m_num;
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
    fd_types[type_].push_back(p);
}

void Reactor::RemoveFd(int fd_, type_t type_)
{
    std::vector<std::pair<int, boost::function<void(int)> > >::iterator iter;
    iter = std::find_if(fd_types[type_].begin(), 
                        fd_types[type_].end(), 
                        FdCompare(fd_));
    if (iter != fd_types[type_].end())
    {
        std::cout << "shoot me\n";
    }
    iter->second = DummyCallback1IMP;                  
}

void Reactor::RemoveAllMarkedElementsIMP()
{
 //   std::cout << "remove all\n";
    for (int i = 0; i < NUM_OF_TYPES; ++i)
    {
        std::vector<std::pair<int, boost::function<void(int)> > >::iterator it;

        for (it = fd_types[i].begin(); it != fd_types[i].end(); ++it)
        {
            if (it->second == DummyCallback1IMP)
            {
                std::cout << "inside if about to erase\n";
                it = fd_types[i].erase(it);
                --it;
            }
        }
    }
}

Reactor::error_t Reactor::Run()
{
    fd_set fdset_arr[NUM_OF_TYPES];
    int maxSockId = 0;

    while (!m_stop)
    {
 //       std::cout << "inside wile m_stop = " << m_stop << "\n";
        maxSockId = GetMaxSocketIMP();

        FD_ZERO(&fdset_arr[0]);
        FD_ZERO(&fdset_arr[1]);
        FD_ZERO(&fdset_arr[2]);
        UpdateFdSetsIMP(&fdset_arr[0], &fdset_arr[1], &fdset_arr[2]);
       
        if (-1 == select(maxSockId + 1, &fdset_arr[0], &fdset_arr[1], &fdset_arr[2], NULL))
        {
            return (SelectHandlerIMP(errno));
        }

        for (int i = 0; i < NUM_OF_TYPES; ++i)
        {
            std::vector<std::pair<int, boost::function<void(int)> > >::
                                                                iterator it;
            std::vector<std::pair<int, boost::function<void(int)> > >::
                                        iterator end = fd_types[i].end();
            for (it = fd_types[i].begin(); it < end; ++it)
            {
                if (FD_ISSET(it->first, &fdset_arr[i]))
                {
                    it->second(it->first);
                }
            }
        }

        RemoveAllMarkedElementsIMP();
    }
   
    return SUCCESS;
}

void Reactor::Stop()
{
    std::cout << "inside stop before changing flag , m_stop = " << m_stop << "\n";
    m_stop = true;
    std::cout << "inside stop after changing flag , m_stop = " << m_stop << "\n";
}

void Reactor::InvokeHandlerIMP(Reactor::type_t type, int sockFd)
{
    std::vector<std::pair<int, boost::function<void(int)> > >::iterator it;

    it = std::find_if(fd_types[type].begin(), 
                      fd_types[type].end(), 
                      FdCompare(sockFd));
    it->second(sockFd);
}

void Reactor::UpdateFdSetsIMP(fd_set *readfds, fd_set *writefds, fd_set *exceptfds)
{
    std::vector<std::pair<int, boost::function<void(int)> > >::iterator it;

    std::for_each(fd_types[READ].begin(), 
                  fd_types[READ].end(), 
                  FdSet(readfds));
                  
    std::for_each(fd_types[WRITE].begin(), 
                  fd_types[WRITE].end(), 
                  FdSet(writefds));

    std::for_each(fd_types[EXCEPT].begin(), 
                  fd_types[EXCEPT].end(), 
                  FdSet(exceptfds));
}

int Reactor::GetMaxSocketIMP()
{
    int maxSocket = 0;
    std::vector<std::pair<int, boost::function<void(int)> > >::iterator it;

    std::for_each(fd_types[READ].begin(), 
                  fd_types[READ].end(), 
                  MaxSocket(&maxSocket));

    std::for_each(fd_types[WRITE].begin(), 
                  fd_types[WRITE].end(), 
                  MaxSocket(&maxSocket));

    std::for_each(fd_types[EXCEPT].begin(), 
                  fd_types[EXCEPT].end(), 
                  MaxSocket(&maxSocket));

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
    void DummyCallback1IMP(int fd)
    {
        std::cout << "Dummy callback\n";
    }

    FdCompare::FdCompare(int fd_)
        : m_fd(fd_)
    {}

    FdCompare::~FdCompare()
    {}

    bool FdCompare::operator()(std::pair<int, boost::function<void(int)> > p)
    {
        return (m_fd == p.first);
    }

    void FdSet::operator()(std::pair<int, boost::function<void(int)> > pr)
    {
        FD_SET(pr.first, m_set);
    }

    void MaxSocket::operator()(std::pair<int, boost::function<void(int)> > pr)
    {
        if (*m_num < pr.first)
        {
            *m_num = pr.first;
        }
    }
} // end of namespace
} // end of namespace ilrd

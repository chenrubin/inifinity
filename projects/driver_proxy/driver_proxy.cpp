#include <sys/types.h>          
#include <sys/socket.h>         // socketpair 
#include <boost/bind.hpp>       // boost::bind

#include "driver_proxy.hpp"
#include "MyUtils.hpp"          // HandleErrorIfExists

#define STDIN (0)

namespace ilrd
{
typedef /*const*/ boost::function<void(const RequestPacketRead&)> read_func_t;
typedef /*const*/ boost::function<void(const RequestPacketWrite&)> write_func_t;

DriverProxy::DriverProxy(Reactor *reactor_, 
                         read_func_t onRead_,
                         write_func_t onWrite_)
    : m_reactor(reactor_)
    , m_onRead(onRead_)
    , m_onWrite(onWrite_)
    , m_thread()
    , m_sock_pair() 
{
    HandleErrorIfExists(0 == socketpair(AF_UNIX, SOCK_STREAM, 0, m_sock_pair) ? 
                        0 : -1, "create socket pair");
    m_reactor->AddFd(m_sock_pair[1], 
                     m_reactor->READ ,
                     boost::bind(&DriverProxy::OnRequestIMP, this, m_sock_pair[1]));
    m_reactor->AddFd(STDIN, 
                     m_reactor->READ ,
                     boost::bind(&DriverProxy::ReactorStopTaskIMP, this, STDIN));
    m_thread = boost::thread(boost::bind(&Reactor::Run, m_reactor));                 
    //m_thread = boost::thread(&Reactor::Run, m_reactor);
}

DriverProxy::~DriverProxy()
{
    m_reactor = NULL;
    m_thread.join();
    close(m_sock_pair[0]);
    close(m_sock_pair[1]);
}

void DriverProxy::OnRequestIMP(int fd_)
{
    std::cout << "OnRequestIMP\n";
}

void DriverProxy::ReactorStopTaskIMP(int fd_)
{
    std::cout << "inside ReactorStopTaskIMP\n";
    char buff[10];
    fgets(buff, sizeof(buff), stdin);
    if (0 == strcmp(buff, "stop\n"))
    {
        std::cout << "Inside strcmp\n";
        m_reactor->Stop();
        std::cout << "Inside strcmp after stop\n";
    }
}

} // end of namespace ilrd
#include <sys/socket.h>         // socketpair 
#include <boost/bind.hpp>       // boost::bind
#include <fcntl.h>              // O_RDWR
#include <linux/nbd.h>          // NBD_SET_SOCK
#include <sys/ioctl.h>          // ioctl

#include "driver_proxy.hpp"
#include "MyUtils.hpp"          // HandleErrorIfExists

#define STDIN (0)

namespace ilrd
{
typedef boost::function<void(const RequestPacketRead&)> read_func_t;
typedef boost::function<void(const RequestPacketWrite&)> write_func_t;

DriverProxy::DriverProxy(Reactor *reactor_, 
                         read_func_t onRead_,
                         write_func_t onWrite_)
    : m_reactor(reactor_)
    , m_onRead(onRead_)
    , m_onWrite(onWrite_)
    , m_thread_run()
    , m_thread_nbd()
    , m_sockPair()
    , m_nbdFd(InitilizeNbdFdIMP())
{
    HandleErrorIfExists(0 == socketpair(AF_UNIX, SOCK_STREAM, 0, m_sockPair) ? 
                        0 : -1, "create socket pair");
    m_reactor->AddFd(m_sockPair[0], 
                     m_reactor->READ ,
                     boost::bind(&DriverProxy::OnRequestIMP, this, m_sockPair[1]));
 /*   m_reactor->AddFd(STDIN, 
                     m_reactor->READ ,
                     boost::bind(&DriverProxy::ReactorStopTaskIMP, this, STDIN));*/
    //m_thread = boost::thread(boost::bind(&Reactor::Run, m_reactor)); 
    NbdConnectionInitializerIMP(m_nbdFd, m_sockPair[1]);
    m_thread_run = boost::thread(&Reactor::Run, m_reactor);
}

DriverProxy::~DriverProxy()
{
    m_reactor = NULL;
    m_thread_run.join();
    m_thread_nbd.join();
    close(m_sockPair[0]);
    close(m_sockPair[1]);
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
int DriverProxy::InitilizeNbdFdIMP()
{
    int nbd = open("/dev/nbd0", O_RDWR);
    HandleErrorIfExists(nbd, "create fd for nbd device path");

    return nbd;
}

void DriverProxy::NbdConnectionInitializerIMP(int fd, int sock)
{
    HandleErrorIfExists(ioctl(fd, NBD_SET_SOCK, sock), "nbd set socket failed");
    HandleErrorIfExists(ioctl(fd, NBD_DO_IT), "nbd do it failed");
}

} // end of namespace ilrd
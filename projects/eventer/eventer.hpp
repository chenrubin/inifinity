// Eventer - Header file
// Last update: 4/5/2020
// Author: 
/*----------------------------------------------------------------------------*/
#ifndef __EVENTER_HPP__
#define __EVENTER_HPP__
/*----------------------------------------------------------------------------*/
#include <boost/function.hpp>   // boost::function
#include <map>                  // std::map

#include "reactor.hpp"          // class ilrd::Reactor
#include "MyUtils.hpp"            // class ilrd::Uncopyable
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
class Eventer: private Uncopyable
{
public:
    explicit Eventer(Reactor *reactor_);
    ~Eventer();

    void SignalEvent(int eventID_);
    void SetEvent(int eventID_, boost::function<void()> callback_);

private:
    void OnPiped(int fd_);
    void CreatePipeIMP();

    Reactor *m_reactor;
    int m_pipeFd[2];
    std::map<int , boost::function<void()> > m_eventsAndCallbacks;
};
/*----------------------------------------------------------------------------*/
}   // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif  // __EVENTER_HPP__


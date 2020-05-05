#include <unistd.h>         // pipe
#include <boost/bind.hpp>   // boosr::bind

#include "eventer.hpp"      // class eventer
//pseudo

namespace ilrd
{
Eventer::Eventer(Reactor *reactor_)
    : m_reactor(reactor_)
    , m_pipeFd()  //Create pipe with fd0(read end) and fd1(write end)
    , m_eventsAndCallbacks()
{
    CreatePipeIMP();
    //Add onPiped to reactor (this will)
    m_reactor->AddFd(m_pipeFd[0], Reactor::READ, boost::bind(&Eventer::OnPiped, this, _1));
}

Eventer::~Eventer()
{
    close(m_pipeFd[0]);
    close(m_pipeFd[1]);
}

// function to be called at the end of the thread's task
void Eventer::SignalEvent(int eventID_)
{
    // notify in the write end of the pipe by sending eventID_
    HandleErrorIfExists(write(m_pipeFd[1], &eventID_, sizeof(int)), 
                         "Failed to write to pipe");
}

void Eventer::SetEvent(int eventID_, boost::function<void()> callback_)
{
    //set this pair inside the m_eventsAndCallbacks
    m_eventsAndCallbacks[eventID_] = callback_;
}

void Eventer::OnPiped(int fd_)
{
    // read from the read end of the pipe to get the eventId
    int readEventId;
    HandleErrorIfExists(read(fd_, &readEventId, sizeof(int)), 
                              "failed to read from pipe");
    // invokes callback that matches the eventID according to map
    m_eventsAndCallbacks[readEventId]();
}

// What will be inside callback?
// The master should have a container that keeps all packets and their
// repective eventId in a map. Keep in mind that when master added task to 
// the thread pool it gave a public incryption function so it has access
// to this map of eventID, packets so it kows which packet to incrypt

void Eventer::CreatePipeIMP()
{
    HandleErrorIfExists(pipe(m_pipeFd), "Failed to create pipe");
}
}
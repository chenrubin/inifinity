// Dispatcher - Header file
// Last update: 26/04/2020
// Author: RD 78-79
// Reviwer: The Mighty Shaddad
/*----------------------------------------------------------------------------*/
#ifndef __DISPATCHER_HPP__ 
#define __DISPATCHER_HPP__ 
/*----------------------------------------------------------------------------*/
#include <algorithm>            // std::find
#include <vector>               // std:vector
#include <boost/bind.hpp>       // boost::bind

#include "MyUtils.hpp"          // uncopyable

namespace ilrd
/*----------------------------------------------------------------------------*/
{
template<typename MSG> 
class BaseCallback;

/*----------------------------------------------------------------------------*/
/*
    Dispatcher is used to communicate bewteen observer and subject.
    Observer conditions:
    1) Must have ObserverCallback class as a member (by value meaning composition ).
       Class will include notify - action func which the observer wishes
       to be invoked when subject notifies regarding a change
       optional - notifyDeath , in case the observer wants function to be invoked 
       before it unregisters from sunject
    Subject conditions:
    1) Must have dispatcher as a member (by value meaning compostion)
    2) Must have folowing public methods:
       - void Register(BaseCallback<MSG>* baseCallback_) which will call 
         dispatcher's Register method
*/
template<typename MSG>
class Dispatcher: private Uncopyable
{
private:    
    friend class BaseCallback<MSG>;
public:
    explicit Dispatcher() NOEXCEPT;
    ~Dispatcher();

    // Inserts basecallbask pointer into container
    // Throws bad_alloc
    void Register(BaseCallback<MSG>* baseCallback_);

    // invokes all container's basecallbacks notify functions
    // Can throw whatever esceptions occur in user's functions
    void Notify(const MSG& message_);

private:
    // Erase basecallbsck from container
    // Throws std::bad_alloc, 
    void Unregister(BaseCallback<MSG>* baseCallback_);

    // assignment of dispatcher pointer to NULL once inside dtor
    // Throws whatever user's notifyDeath may throw  
    void NullingDispatcher(BaseCallback<MSG>* baseCallback_);

    std::vector<BaseCallback<MSG>* > m_callbacks;
};
/*----------------------------------------------------------------------------*/
template<typename MSG>
class BaseCallback
{
private:    
    friend class Dispatcher<MSG>;
public:
    explicit BaseCallback() NOEXCEPT;
    virtual ~BaseCallback();

private:
    virtual void Notify(const MSG& message_) = 0;
    virtual void NotifyDispatcherDeath() = 0;

    Dispatcher<MSG> *m_dispatcher;
};
/*----------------------------------------------------------------------------*/
template<typename MSG, typename OBSV>
class ObserverCallback: public BaseCallback<MSG>
{
public:
    /* Arguments are:
    - observer pointer
    - function to notify observer each time there is a notification from subject
    - optional function to invoke when dispatcher is destoryed and neds to 
      inform observer
    */
    explicit ObserverCallback(OBSV* observer_, 
                              void (OBSV::*obsNotify_)(const MSG&), 
                              void (OBSV::*obsBeforeDeath_)() = 0);
    virtual ~ObserverCallback();

private:
    typedef void (OBSV::*obsNotify_t)(const MSG&);
    typedef void (OBSV::*obsBeforeDeath_t)();
    
    // invoke observer m_notify
    // Can throw whatever esceptions occur in user's functions
    virtual void Notify(const MSG& message_);

    // invoke observer m_death inside
    // Can throw whatever esceptions occur in user's function
    virtual void NotifyDispatcherDeath();    

    OBSV* m_observer;
    obsNotify_t m_notify;
    obsBeforeDeath_t m_death;
};

/******************************************************************************
 *          Dispatcher methods
******************************************************************************/ 
template <typename MSG>
Dispatcher<MSG>::Dispatcher()
    : m_callbacks()
{}

template <typename MSG>
Dispatcher<MSG>::~Dispatcher()
{
    for (size_t i = 0; i < m_callbacks.size(); ++i)
    {
        NullingDispatcher(m_callbacks[i]);
    }
}

template <typename MSG>
void Dispatcher<MSG>::NullingDispatcher(BaseCallback<MSG>* baseCallback_)
{
    baseCallback_->NotifyDispatcherDeath();
    baseCallback_->m_dispatcher = 0;
}

template <typename MSG>
void Dispatcher<MSG>::Register(BaseCallback<MSG>* baseCallback_)
{
    Dispatcher* belongDispatcher = baseCallback_->m_dispatcher;
	if (this != belongDispatcher && 0 != belongDispatcher)
	{
		belongDispatcher->Unregister(baseCallback_);
	}
    else if (0 == belongDispatcher)
    {
        baseCallback_->m_dispatcher = this;
	    m_callbacks.push_back(baseCallback_);
    }
}

template <typename MSG>
void Dispatcher<MSG>::Notify(const MSG& message_)
{
    for (size_t i = 0; i < m_callbacks.size(); ++i)
    {
        BaseCallback<MSG> *next_BaseCallback = 0;

        if ((i + 1) < m_callbacks.size())
        {
            next_BaseCallback = m_callbacks[i + 1];
        }
        
        m_callbacks[i]->Notify(message_);

        if (next_BaseCallback == m_callbacks[i])
        {
            --i;
        }
    }
/*    typename std::vector<BaseCallback<MSG>* >::iterator iter;
    for (iter = m_callbacks.begin(); iter != m_callbacks.end(); ++iter)
	{
		(*iter)->Notify(message_);
	}*/
}

template <typename MSG>
void Dispatcher<MSG>::Unregister(BaseCallback<MSG>* baseCallback_)
{
    typename std::vector<BaseCallback<MSG>* >::iterator iter;

    iter = std::find(m_callbacks.begin(), m_callbacks.end(), baseCallback_);
    if (iter != m_callbacks.end())
    {
	    m_callbacks.erase(iter);
    }
    
}
/******************************************************************************
 *          BaseCAllback methods
******************************************************************************/
template <typename MSG>
BaseCallback<MSG>::BaseCallback()
    : m_dispatcher()
{}

template <typename MSG>
BaseCallback<MSG>::~BaseCallback()
{
    if (0 != m_dispatcher)
    {
        m_dispatcher->Unregister(this);
    }
}

/******************************************************************************
 *          ObserverCallback methods
******************************************************************************/
template <typename MSG, typename OBSV>
ObserverCallback<MSG, OBSV>::ObserverCallback(OBSV* observer_, 
                                        void (OBSV::*obsNotify_)(const MSG&), 
                              	        void (OBSV::*obsBeforeDeath_)())
	: m_observer(observer_)
    , m_notify(obsNotify_)
    , m_death(obsBeforeDeath_)
{}

template <typename MSG, typename OBSV>
ObserverCallback<MSG, OBSV>::~ObserverCallback()
{}

template <typename MSG, typename OBSV>
void ObserverCallback<MSG, OBSV>::Notify(const MSG& message_)
{
	(m_observer->*m_notify)(message_);
}

template <typename MSG, typename OBSV>
void ObserverCallback<MSG, OBSV>::NotifyDispatcherDeath()
{
    if (NULL != m_death)
    {
       (m_observer->*m_death)();
    }
}
/*----------------------------------------------------------------------------*/
}   // end namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __DISPATCHER_HPP__
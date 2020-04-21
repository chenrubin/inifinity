// Reactor - Header file
// Last update: 17/03/2020
// Author: Sasha & Ran = love4ever
// Reviewer: 
/*----------------------------------------------------------------------------*/
#ifndef __REACTOR_HPP__ 
#define __REACTOR_HPP__
/*----------------------------------------------------------------------------*/
#include <map>               // std::vector
#include <utility>              // std::pair
#include <boost/function.hpp>   // boost::function
#include <boost/noncopyable.hpp> //noncopyable
#include <cerrno> // errno

#define PAIR std::pair<int, boost::function<void(int)> > 
#define FUNC boost::function<void(int)>
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
class Reactor : private boost::noncopyable  // not thread safe
{
public:
    explicit Reactor(); //v
    ~Reactor(); //v
    
    enum type_t 
    { 
        READ = 0, 
        WRITE = 1, 
        EXCEPT = 2,
        SIZE_TYPE_T
    };
    void AddFd(int fd_, type_t type_, boost::function<void(int)> callback_); 
                         // throws bad alloc, callback throw types
    void RemoveFd(int fd_, type_t type_);
        
    enum error_t
    {
		SUCCESS = 0,
    	E_EBADF = EBADF,
        E_EINTR = EINTR
    }; 
    error_t Run(); // throws exceptions from callback_ , from select
        
    void Stop();

private:
    std::map<int, FUNC> m_pairsFdPlusFunctor[SIZE_TYPE_T];  // change to map
	fd_set m_sets[SIZE_TYPE_T];
    fd_set m_copySets[SIZE_TYPE_T];

	int m_fdMax;
    bool m_run;
	
	void HandleAllRequests();
    void PrepareCopySets();
    bool ReadyToRun();
    
    void UpdateFdMax();
    void UpdateVectors();
};
//--------------------------------------------------------------------------~~~~
//--------------------------------------------------------------------------~~~~

} // namespace ilrd
#endif // __REACTOR_HPP__
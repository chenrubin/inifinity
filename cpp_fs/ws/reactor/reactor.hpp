// Reactor - Header file
// Last update: 17/03/2020
// Author: RD 78-79
// Reviewer: 
/*----------------------------------------------------------------------------*/
#ifndef __REACTOR_HPP__ 
#define __REACTOR_HPP__
/*----------------------------------------------------------------------------*/
#include <vector>				// std::vector
#include <boost/function.hpp>	// boost::function

#include "MyUtils.hpp"		// class Uncopyable
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
class Reactor : private Uncopyable	// not thread safe
{
public:
	explicit Reactor();
	~Reactor();
	
	// callback_ must be short, non blocking function
	// callback_ might also call AddFd / RemoveFd / Stop
	enum type_t { READ, WRITE, EXCEPT };

	// throw if push to vector fails
	void AddFd(int fd_, type_t type_, boost::function<void(int)> callback_);	

	// throw if not found 
	void RemoveFd(int fd_, type_t type_);
	
	// throws exceptions from callback_ / returns errors from select
	enum error_t { /* TODO */ };
	error_t Run();
	
	// if not running - throw exception ?
	void Stop();

private:
	std::vector<std::pair<int, boost::function<void(int)> > > m_readFds;
	std::vector<std::pair<int, boost::function<void(int)> > > m_writeFds;
	std::vector<std::pair<int, boost::function<void(int)> > > m_exceptFds;
	bool m_stop;
};
/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __REACTOR_HPP__

/*int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
*/
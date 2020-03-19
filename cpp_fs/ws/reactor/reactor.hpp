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

#define NUM_OF_TYPES 3
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
	enum error_t 
	{ 
		SUCCESS = 0,
		EBADF_T = 1,
		EINTR_T = 2,
		EINVAL_T = 3,
		ENOMEM_T = 4
	};
	
	error_t Run();
	
	// if not running - throw exception ?
	void Stop();

private:
	std::vector<std::pair<int, boost::function<void(int)> > > fd_types[NUM_OF_TYPES];
	bool m_stop;

	void UpdateFdSetsIMP(fd_set *read, fd_set *write, fd_set *except);
	int GetMaxSocketIMP();
	void InvokeHandlerIMP(Reactor::type_t type, int sockFd);
	error_t SelectHandlerIMP(int err);
};
/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __REACTOR_HPP__
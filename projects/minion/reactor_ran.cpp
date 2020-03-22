#include <iterator> // iterator
#include <numeric> // accumulate
#include <exception> //exception

// select-related
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#include "reactor.hpp"

namespace ilrd
{

/******************************************************************************/
// Auxiliary Functions Declarations:
/******************************************************************************/
namespace
{
// Unary:
void MacroFD_ZERO(fd_set &set);
size_t IsNotEmptyVector(std::vector<PAIR> vector);
// Binary:
fd_set DoHandlingIMP(std::vector<PAIR> pairsFdsAndFunctrors, fd_set &set_);
bool CompareFds(PAIR pair1, PAIR pair2);
// Auxiliaary:
Reactor::error_t HandleErrors(bool *needToHandle, size_t *errorCounter);
fd_set ZeroizeSet(fd_set set);

/******************************************************************************/
// Functors Classes
/******************************************************************************/
// Is Match
/******************************************************************************/
class IsMatchFd
{
public:
    IsMatchFd(int fd_)
    : m_fd(fd_)
    { }
    ~IsMatchFd()
    { }

    bool operator()(PAIR &pair_)
    {
        return m_fd == pair_.first;
    }
private:
    int m_fd;
};
/******************************************************************************/
// IsMatchFd
/******************************************************************************/
class MacroFDset
{
public:
    MacroFDset(fd_set  *set_)
    : m_set(set_)
    { }
    ~MacroFDset()
    { }

    void operator()(PAIR &pair_)
    {
        FD_SET(pair_.first, m_set);
    }
private:
    fd_set *m_set;
};
/******************************************************************************/
class HandleSingleRequest
{
public: 
	HandleSingleRequest(fd_set *set_)
	: m_set(set_)
	{ }
	
	~HandleSingleRequest()
	{ }

	void operator()(PAIR &input_) // throws callback throw
	{
		if (FD_ISSET(input_.first, m_set))
		{
			input_.second(input_.first);
		}
	}
private:
	fd_set *m_set;
};
} //anonymous
/******************************************************************************/
// Class Functions:
/******************************************************************************/
 Reactor::Reactor()
	: m_fdMax(-1)
	, m_run(true)
{
	ZeroizeSets(m_sets);
}
/******************************************************************************/
Reactor::~Reactor()
{ }
/******************************************************************************/
void Reactor::AddFd(int fd_, typename Reactor::type_t type_
            , boost::function<void(int)> callback_)
{
    PAIR pair(fd_, callback_);
    m_pairsFdPlusFunctor[type_].push_back(pair);
	if (fd_ > m_fdMax)
	{
		m_fdMax = fd_;
	}
	FD_SET(pair.first, &m_sets[type_]);
}
/******************************************************************************/

void Reactor::RemoveFd(int fd_, type_t type_)
{
    std::vector<PAIR>::iterator it
    (std::find_if(m_pairsFdPlusFunctor[type_].begin()
                , m_pairsFdPlusFunctor[type_].end(),IsMatchFd(fd_)));

    if(it != m_pairsFdPlusFunctor[type_].end())
    {
		m_pairsFdPlusFunctor[type_].erase(it);
    }
	FD_CLR(fd_, &m_sets[type_]);
	UpdateFdMax(fd_);
}
/******************************************************************************/

Reactor::error_t Reactor::Run()
{
	m_run = ReadyToRun() ? true : false;
	while(m_run)
	{
		fd_set copySets[SIZE_TYPE_T];
		ZeroizeSets(copySets);
		std::copy(m_sets, m_sets + SIZE_TYPE_T, copySets);
		int status = 
		select(m_fdMax + 1, &copySets[READ], &copySets[WRITE]
				, &copySets[EXCEPT], NULL);

		bool needToHandle = true;
		static size_t errorCounter = 0;
		if (-1 == status)
		{
			if(SUCCESS != HandleErrors(&needToHandle, &errorCounter))
			{
				return (Reactor::error_t)errno;
			}
		}

		if(needToHandle)
		{
			HandleAllRequests(copySets);
			errorCounter = 0;
		}
	}
	return  SUCCESS;
}
/******************************************************************************/

void Reactor::Stop()
{
	m_run = false;
}
/******************************************************************************/
// Private funcs
/******************************************************************************/
bool Reactor::ReadyToRun()
{
	size_t unemptiness[SIZE_TYPE_T] = { 0 };
	std::transform(m_pairsFdPlusFunctor, m_pairsFdPlusFunctor + SIZE_TYPE_T, 
					unemptiness, IsNotEmptyVector);
	return ((std::accumulate(unemptiness, unemptiness + SIZE_TYPE_T, 0)) != 0);
}
/******************************************************************************/

void Reactor::HandleAllRequests(fd_set copySets_[SIZE_TYPE_T])
{
	std::transform(m_pairsFdPlusFunctor, m_pairsFdPlusFunctor + SIZE_TYPE_T, 
				   copySets_, copySets_, DoHandlingIMP);
}
/******************************************************************************/

void Reactor::UpdateFdMax(int removedFd_)
{
	if (removedFd_ == m_fdMax)
	{
		int fdMaxArray[SIZE_TYPE_T] = { 0 };
		for (size_t index = 0; index < SIZE_TYPE_T; ++index)
		{
			std::vector<PAIR>::iterator it = 
			std::max_element(m_pairsFdPlusFunctor[index].begin(),
							m_pairsFdPlusFunctor[index].end(),
							CompareFds);
			if (it != m_pairsFdPlusFunctor[index].end())
			{
				fdMaxArray[index] = it->first;
			}

		}
		m_fdMax = *(std::max_element(fdMaxArray, fdMaxArray + SIZE_TYPE_T));
	}
}
/******************************************************************************/
// Unary:
namespace
{
/******************************************************************************/
void MacroFD_ZERO(fd_set &set)
{
	FD_ZERO(&set);
}
/******************************************************************************/

size_t IsNotEmptyVector(std::vector<PAIR> vector)
{
	return (size_t) !vector.empty();
}
/******************************************************************************/
// Binary:
/******************************************************************************/

fd_set DoHandlingIMP(std::vector<PAIR> pairsFdsAndFunctrors, fd_set &set_)
{
	std::for_each(pairsFdsAndFunctrors.begin(), pairsFdsAndFunctrors.end()
				,HandleSingleRequest(&set_));
	return set_;
}
/******************************************************************************/
bool CompareFds(PAIR pair1, PAIR pair2)
{
	return pair1.first < pair2.first;
}
/******************************************************************************/
//Auxiliary:
/******************************************************************************/
void ZeroizeSets(fd_set set[Reactor::SIZE_TYPE_T])
{
	std::for_each(set, set + Reactor::SIZE_TYPE_T, MacroFD_ZERO);
}


Reactor::error_t HandleErrors(bool *needToHandle, size_t *errorCounter)
{
	switch(errno)
		{
			case EBADF:
				*needToHandle = false;
				++*errorCounter;
			break;
			case EINTR:
				*needToHandle = false;
				++*errorCounter;
			break;
			case EINVAL:
				throw std::out_of_range("fd corrupted or timeout");
			break;
			case ENOMEM:
				throw std::bad_alloc();
			break;
			default:
			break;
		}

		if (5 == *errorCounter)
		{
			return (Reactor::error_t)errno;
		}
		return Reactor::SUCCESS;
}
}// anonymous
} // namespace ilrd
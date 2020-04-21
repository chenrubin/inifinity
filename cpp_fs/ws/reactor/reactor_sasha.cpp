#include <iterator> // iterator
#include <numeric> // accumulate
#include <exception> //exception

// select-related
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#include "reactor.hpp"
//#include "cpp_std.hpp"

namespace ilrd
{

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Auxiliary Functions Declarations:
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Unary:
namespace
{
void MacroFD_ZERO(fd_set &set);
size_t IsNotEmptyMap(std::map<int, FUNC> vector);
// Binary:
fd_set DoHandlingIMP(std::map<int, FUNC> pairsFdsAndFunctrors, fd_set &set_);
bool CompareFds(PAIR pair1, PAIR pair2);
// Auxiliary:
Reactor::error_t HandleErrors(bool *needToHandle, size_t *errorCounter);

void ZeroizeSets(fd_set sets[Reactor::SIZE_TYPE_T]);
void EmptyCallback(int i)
{
	(void)i;
}
//--------------------------------------------------------------------------~~~~
//--------------------------------------------------------------------------~~~~

class HandleSingleRequest
{
public: 
	HandleSingleRequest(fd_set *set_)
	: m_set(set_)
	{ }
	
	~HandleSingleRequest()
	{ }

	void operator()(std::pair<const int, FUNC> &input_) // throws callback throw
	{
		if (FD_ISSET(input_.first, m_set))
		{
			input_.second(input_.first);
		}
	}
private:
	fd_set *m_set;
};
//--------------------------------------------------------------------------~~~~
} //anonymous

//--------------------------------------------------------------------------~~~~
// Class Functions:
//--------------------------------------------------------------------------~~~~
//--------------------------------------------------------------------------~~~~

//--------------------------------------------------------------------------~~~~
// Ctor/ Dtor
//--------------------------------------------------------------------------~~~~
 Reactor::Reactor()
	: m_fdMax(-1) // pessimistic -1!
	, m_run(false) // optimistic false!
{
	ZeroizeSets(m_sets);
}
//--------------------------------------------------------------------------~~~~

Reactor::~Reactor()
{ }

//--------------------------------------------------------------------------~~~~
// Add/Remove
//--------------------------------------------------------------------------~~~~
void Reactor::AddFd(int fd_, typename Reactor::type_t type_
            , boost::function<void(int)> callback_)
{
    m_pairsFdPlusFunctor[type_].insert(std::pair<int,FUNC>(fd_, callback_));
	if (fd_ > m_fdMax)
	{
		m_fdMax = fd_;
	}
	FD_SET(fd_, &m_sets[type_]);
}
//--------------------------------------------------------------------------~~~~

void Reactor::RemoveFd(int fd_, type_t type_)
{
    std::map<int, FUNC>::iterator it(m_pairsFdPlusFunctor[type_].find(fd_));

    if(it != m_pairsFdPlusFunctor[type_].end())
    {
		it->second = EmptyCallback;
    }
	FD_CLR(fd_, &m_sets[type_]);
}
//--------------------------------------------------------------------------~~~~
// Run/Stop
//--------------------------------------------------------------------------~~~~

Reactor::error_t Reactor::Run()
{
	m_run = ReadyToRun() ? true : false;
	while(m_run)
	{
		PrepareCopySets();
		int status = 
		select(m_fdMax + 1, &m_copySets[READ], &m_copySets[WRITE],
													 &m_copySets[EXCEPT], NULL);
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
			HandleAllRequests();
			errorCounter = 0;
		}

		UpdateVectors();
		UpdateFdMax();
	}
	return  SUCCESS;
}
//--------------------------------------------------------------------------~~~~

void Reactor::Stop()
{
	m_run = false;
}
//--------------------------------------------------------------------------~~~~
// Private funcs
//--------------------------------------------------------------------------~~~~
bool Reactor::ReadyToRun()
{
	size_t unemptiness[SIZE_TYPE_T] = { 0 };
	std::transform(m_pairsFdPlusFunctor, m_pairsFdPlusFunctor + SIZE_TYPE_T, 
												unemptiness, IsNotEmptyMap);
	return (std::accumulate(unemptiness, unemptiness + SIZE_TYPE_T, 0)) != 0;
}
//--------------------------------------------------------------------------~~~~

void Reactor::HandleAllRequests()
{
	std::transform(m_pairsFdPlusFunctor, m_pairsFdPlusFunctor + SIZE_TYPE_T, 
										m_copySets, m_copySets, DoHandlingIMP);
}
//--------------------------------------------------------------------------~~~~

void Reactor::UpdateFdMax()
{
	int fdMaxArray[SIZE_TYPE_T] = { 0 };
	for (size_t i = 0; i < SIZE_TYPE_T; ++i)
	{
		std::map<int, FUNC>::iterator it = 
		std::max_element(m_pairsFdPlusFunctor[i].begin(), 
							m_pairsFdPlusFunctor[i].end(), 
															CompareFds);
		if (it != m_pairsFdPlusFunctor[i].end())
		{
			fdMaxArray[i] = it->first;
		}

	}
	m_fdMax = *(std::max_element(fdMaxArray, fdMaxArray + SIZE_TYPE_T));
	
}

bool HasEmptyCallback(std::pair<const int,FUNC> pair)
{
	return pair.second == EmptyCallback;
}

void UpdateMap(std::map<int,FUNC> &map)
{
	std::map<int, FUNC>::iterator it (
	std::find_if(map.begin(), map.end(), HasEmptyCallback));
	if (map.end() != it)
	{
		map.erase(it);
	}
}

void Reactor::UpdateVectors()
{
	std::for_each(m_pairsFdPlusFunctor, m_pairsFdPlusFunctor + SIZE_TYPE_T,
																	 UpdateMap);
}

void Reactor::PrepareCopySets()
{
	ZeroizeSets(m_copySets);
	std::copy(m_sets, m_sets + SIZE_TYPE_T, m_copySets);
}

//--------------------------------------------------------------------------~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Unary:
namespace
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MacroFD_ZERO(fd_set &set)
{
	FD_ZERO(&set);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

size_t IsNotEmptyMap(std::map<int, FUNC> map)
{
	return (size_t) !map.empty();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Binary:
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

fd_set DoHandlingIMP(std::map<int, FUNC> pairsFdsAndFunctrors, fd_set &set_)
{
	std::for_each(pairsFdsAndFunctrors.begin()
                , pairsFdsAndFunctrors.end()
												,HandleSingleRequest(&set_));
	return set_;
}

bool CompareFds(PAIR pair1, PAIR pair2)
{
	return pair1.first < pair2.first;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Auxiliary:
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void ZeroizeSets(fd_set sets[Reactor::SIZE_TYPE_T])
{
	std::for_each(sets, sets + Reactor::SIZE_TYPE_T, MacroFD_ZERO);
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

		if (*errorCounter == 5)
		{
			return (Reactor::error_t)errno;
		}
		return Reactor::SUCCESS;
}
}// anonymous
} // namespace ilrd
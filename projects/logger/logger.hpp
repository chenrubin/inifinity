// Logger - Header file
// Last update: 29/03/2020
// Author: RD 78-79
// Reviewer: Kewan
/*----------------------------------------------------------------------------*/
#ifndef __LOGGER_HPP__ 
#define __LOGGER_HPP__
/*----------------------------------------------------------------------------*/
#include <fstream>    			/* fstream */
#include <boost/atomic.hpp>    	/* atomic */
#include <boost/thread.hpp> 	/* boost::thread */
#include <queue>				/* queue */

#include "singleton.hpp"
#include "MyUtils.hpp"						// class Uncopyable, HandleErrors
#include "waitable_queue.hpp"
/*----------------------------------------------------------------------------*/
#define LOG_DEBUG(msg) (Logger::GetLogger()->Log(Logger::DEBUG, msg))
#define LOG_INFO(msg) (Logger::GetLogger()->Log(Logger::INFO, msg))
#define LOG_WARNING(msg) (Logger::GetLogger()->Log(Logger::WARNING, msg))
#define LOG_ERROR(msg) (Logger::GetLogger()->Log(Logger::ERROR, msg))
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
class Logger : private Uncopyable
{
public:
	// throw (std::bad_aloc)
	static Logger* GetLogger() 
	{
		return Singleton<Logger>::GetInstance();
	}

	~Logger();

	enum Severity { DEBUG = 0, INFO, WARNING, ERROR, NUM_OF_SEVERITIES };

	// Adds a message to the log, if its severity is higher enough.
	void Log(Severity msgSeverity_, const std::string& msg_) NOEXCEPT;

	// Changes the mininal severity required to log a message.
	void SetMinimalSeverity(Severity minimalSeverity_) NOEXCEPT;

private:
	friend class Singleton<Logger>;
	explicit Logger();

	char *GetStreamNameIMP();
	void PopThreadRoutineIMP();

	WaitableQueue<std::queue<std::string> > m_queue;
	Severity m_minimalSeverity;
	boost::atomic<bool> m_isThreadActivated;

	// The user must create an envioronment variable LOGGER_FILE with the name
	// of the desired file
	std::ofstream m_outputStream;
	boost::thread m_pop_thread;
};
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __LOGGER_HPP__
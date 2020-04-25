// Logger - Header file
// Last update: 29/03/2020
// Author: RD 78-79
// Reviewer: 
/*----------------------------------------------------------------------------*/
#ifndef __LOGGER_HPP__ 
#define __LOGGER_HPP__
/*----------------------------------------------------------------------------*/
#include <fstream>				// ofstream
#include <string>				// string
#include <queue>				// queue
#include <boost/thread.hpp>     // boost::thread
#include <boost/atomic.hpp>		// boost::atomic

#include "singleton.hpp"		// Singleton
#include "waitable_queue.hpp"	// WaitableQueue
#include "MyUtils.hpp"			// class Uncopyable
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
#define LOG_DEBUG(msg) Logger::GetLogger()->Log(Logger::DEBUG, msg)
#define LOG_INFO(msg) Logger::GetLogger()->Log(Logger::INFO, msg)
#define LOG_WARNING(msg) Logger::GetLogger()->Log(Logger::WARNING, msg)
#define LOG_ERROR(msg) Logger::GetLogger()->Log(Logger::ERROR, msg)
/*----------------------------------------------------------------------------*/
class Logger : private Uncopyable
{
public:
	// The message severity is set to DEBUG by default
	// Environment variable LOG_FILE_PATH must be set to the path of file
	// throws exception of 'runtime_error' if no enironment variable was set
	// throws exception of 'failure' if opening file fails
	static Logger* GetLogger();

	~Logger() NOEXCEPT;

	enum Severity { DEBUG = 0, INFO, WARNING, ERROR, NUM_OF_SEVERITIES };

	// Adds a message to the log, if its severity is higher enough.
	// might throw boost::thread_resource_error
	void Log(Severity msgSeverity_, const std::string& msg_);

	// Changes the mininal severity required to log a message.
	void SetMinimalSeverity(Severity minimalSeverity_) NOEXCEPT;

private:
	friend class Singleton<Logger>;

	explicit Logger();

	void InitArrOfPushIMP();
	void PushDetailsIMP(const std::string& msg_);
	void PushNothingIMP(const std::string& msg_);
	void PopIMP();
	
	typedef void (Logger::*PushFuncs_t)(const std::string& msg_);	
	
	PushFuncs_t push_funcs_arr[2];
	
	std::ofstream m_file;
	boost::atomic<bool> is_stop;
	Severity m_SeverityLevel;
	WaitableQueue<std::queue<std::string> > m_queue;
	boost::thread m_pop_thread;
};
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __LOGGER_HPP__
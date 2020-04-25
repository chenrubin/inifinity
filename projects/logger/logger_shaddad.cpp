/*********************************
 * Author: Shaddad Gomid         *
 * Logger                        *
 * Reviewer: Tsisana             *
 * Date: April 1, 2020           *
 * ******************************/
#include <cstdlib>      // getenv
#include <stdexcept>    // std::runtime_error

#include "logger.hpp"

/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
namespace
{
const std::string svrties_arr[Logger::NUM_OF_SEVERITIES] = 
                                  {"DEBUG: ", "INFO: ", "WARNING: ", "ERROR: "};

inline void CheckValue(void* val, const std::string& msg);
}
/*----------------------------------------------------------------------------*/
Logger::Logger()
    : push_funcs_arr()
    , m_file()
    , is_stop(false)
    , m_SeverityLevel(DEBUG)
    , m_queue()
    , m_pop_thread()
{
    char* log_file_path = getenv("LOG_FILE_PATH");
    CheckValue(log_file_path, "environment variable is invalid");

    m_file.open(log_file_path, std::ofstream::out);
    InitArrOfPushIMP();

    m_pop_thread = boost::thread(&Logger::PopIMP, this);
}

Logger::~Logger() NOEXCEPT
{
    while(!m_queue.IsEmpty())
    {
        boost::this_thread::yield();
    }
    is_stop = true;
    
    std::string msg("");
    m_queue.Push(msg);   // to get the popping-thread out of blocking
    
    m_pop_thread.join();
    m_file.close();
}

Logger* Logger::GetLogger()
{
    return Singleton<Logger>::GetInstance();
}

void Logger::Log(Severity msgSeverity_, const std::string& msg_)
{
    bool is_msg_worth_pushing = (msgSeverity_ >= m_SeverityLevel);
    std::string msg_info = "Message severity " 
                         + svrties_arr[msgSeverity_] 
                         + msg_;
                         
    (this->*push_funcs_arr[is_msg_worth_pushing])(msg_info);
}

void Logger::SetMinimalSeverity(Severity minimalSeverity_) NOEXCEPT
{
    m_SeverityLevel = minimalSeverity_;
}

/*----------------------------------------------------------------------------*/
// Helping functions

void Logger::InitArrOfPushIMP()
{
    push_funcs_arr[0] = &Logger::PushNothingIMP;
    push_funcs_arr[1] = &Logger::PushDetailsIMP;
}

void Logger::PushDetailsIMP(const std::string& msg_)
{
    m_queue.Push(const_cast<std::string&>(msg_));
}

void Logger::PushNothingIMP(const std::string& msg_)
{
    /* Empty body */
}

void Logger::PopIMP()
{
    while (!is_stop)
    {
        std::string msg_to_get;
        m_queue.Pop(msg_to_get);
        m_file << msg_to_get;
    }
}

namespace
{
inline void CheckValue(void* val, const std::string& msg)
{
    if (0 == val)
    {
        throw std::runtime_error(msg);
    }
}
}
/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
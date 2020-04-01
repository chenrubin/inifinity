#include <cstdlib>
#include <pthread.h>    /* pthread */
#include <boost/chrono.hpp> /* boost::chrono */

#include "logger.hpp"

namespace ilrd
{
Logger::Logger()
    : m_minimalSeverity(DEBUG)
    , m_isThreadActivated(true)
    , m_outputStream(GetStreamNameIMP(), std::ofstream::out | std::ofstream::app)
    , m_queue()
    , m_pop_thread(&Logger::PopThreadRoutineIMP, this)
{}

Logger::~Logger()
{
    m_isThreadActivated.store(false, boost::memory_order_release);
    m_pop_thread.join();
    m_outputStream.close();
}

void Logger::Log(Severity msgSeverity_, const std::string& msg_) NOEXCEPT
{
    if (msgSeverity_ >= m_minimalSeverity)
    {
        m_queue.Push(msg_);
    }
}

void Logger::SetMinimalSeverity(Severity minimalSeverity_) NOEXCEPT
{
    m_minimalSeverity = minimalSeverity_;
}

char *Logger::GetStreamNameIMP()
{
    char *file_name = std::getenv("LOGGER_FILE");

    HandleErrorIfExists(file_name == NULL ? -1 : 0, "File doesn't exist");

    return file_name;;
}

void Logger::PopThreadRoutineIMP()
{
    while (m_isThreadActivated.load(boost::memory_order_acquire))
    {
        std::string str;
        bool status = m_queue.Pop(str, boost::chrono::milliseconds(1000));
        if (status == true)
        {
            m_outputStream << str << std::endl;
        }
    }
}
} // end of namespace ilrd
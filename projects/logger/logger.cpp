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
{
    std::cout << "logger inside Ctor\n";
}

Logger::~Logger()
{
    std::cout << "logger inside ~Logger\n";
    m_isThreadActivated.store(false, boost::memory_order_release);
    m_pop_thread.join();
    m_outputStream.close();
}

void Logger::Log(Severity msgSeverity_, const std::string& msg_) NOEXCEPT
{
    std::cout << "logger inside Log\n";
    if (msgSeverity_ >= m_minimalSeverity)
    {
        m_queue.Push(msg_);
    }
}

void Logger::SetMinimalSeverity(Severity minimalSeverity_) NOEXCEPT
{
    m_minimalSeverity = minimalSeverity_;
    std::cout << "logger inside set minimal severity\n";
}

char *Logger::GetStreamNameIMP()
{
    char *file_name = std::getenv("LOGGER_FILE");

    HandleErrorIfExists(file_name == NULL ? -1 : 0, "File doesn't exist");

    return file_name;;
}

void Logger::PopThreadRoutineIMP()
{
    std::cout << "logger inside pop thrad routinme\n";
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
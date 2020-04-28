#include <cstdlib>
#include <pthread.h>    /* pthread */
#include <boost/chrono.hpp> /* boost::chrono */

#include "logger.hpp"

namespace ilrd
{
Logger::Logger()
    : m_queue()
    , m_minimalSeverity(DEBUG)
    , m_isThreadActivated(/*true*/false)
    , m_outputStream(/*GetStreamNameIMP(), std::ofstream::out/* | std::ofstream::app*/)
//    , m_queue()
    , m_pop_thread()//m_pop_thread(boost::bind(&Logger::PopThreadRoutineIMP, this))//m_pop_thread(&Logger::PopThreadRoutineIMP, this)
{
    m_outputStream.open(GetStreamNameIMP());
    m_pop_thread = boost::thread(&Logger::PopThreadRoutineIMP, this);
 //   std::cout << "logger inside Ctor\n";
}

Logger::~Logger()
{
//    std::cout << "logger inside ~Logger\n";
    m_isThreadActivated.store(false, boost::memory_order_release);
    m_pop_thread.join();
    m_outputStream.close();
}

void Logger::Log(Severity msgSeverity_, const std::string& msg_) NOEXCEPT
{
  //  std::cout << "logger inside Log\n";
    if (msgSeverity_ >= m_minimalSeverity)
    {
        m_queue.Push(msg_);
    }
}

void Logger::SetMinimalSeverity(Severity minimalSeverity_) NOEXCEPT
{
    m_minimalSeverity = minimalSeverity_;
//    std::cout << "logger inside set minimal severity\n";
}

char *Logger::GetStreamNameIMP()
{
    char *file_name = std::getenv("LOGGER_FILE");

    HandleErrorIfExists(file_name == NULL ? -1 : 0, "File doesn't exist");

    return file_name;;
}

void Logger::PopThreadRoutineIMP()
{
//    std::cout << "logger inside pop thread routinme\n";
    m_isThreadActivated.store(true);
    while (m_isThreadActivated.load(boost::memory_order_acquire))
    {
      //  std::cout << "WTF1\n";
        std::string str;
     //   std::cout << "before pop\n";
        bool status = m_queue.Pop(str, boost::chrono::milliseconds(1));
      //  std::cout << "after pop\n";

        if (status == true)
        {
    //        std::cout << "inside status == true\n";
            m_outputStream << str << std::endl;
        }
      //  std::cout << "WTF2\n";
    }
}
} // end of namespace ilrd
#include <iostream>
#include <boost/thread.hpp> /* boost::thread */

#include "logger.hpp"


using namespace ilrd;

void TestSimple();
void TestWithThreads();
void ThreadPushDEBUGRoutine(Logger *logger, std::string str);
void ThreadPushINFORoutine(Logger *logger, std::string str);
void ThreadPushWARNINGRoutine(Logger *logger, std::string str);
void ThreadPushERRORRoutine(Logger *logger, std::string str);

int main()
{
    TestSimple();
    TestWithThreads();
}

void TestSimple()
{
    setenv("LOGGER_FILE", "/home/chenr/git/projects/logger/logger.txt", 1);

    std::string debug_str = "This is debug str";
    std::string info_str = "This is info str";
    std::string warning_str = "This is warning str";
    std::string error_str = "This is error str";

    
    Logger *log = Logger::GetLogger();

    log->SetMinimalSeverity(log->WARNING);
    LOG_DEBUG(debug_str);
    LOG_INFO(info_str);
    LOG_WARNING(warning_str);
    LOG_ERROR(error_str);

    sleep(3);
}
  
void TestWithThreads()
{
    setenv("LOGGER_FILE", "/home/chenr/git/projects/logger/logger.txt", 1);

    std::string debug_str = "This is debug str";
    std::string info_str = "This is info str";
    std::string warning_str = "This is warning str";
    std::string error_str = "This is error str";
    
    Logger *log = Logger::GetLogger();

    log->SetMinimalSeverity(log->INFO);

    boost::thread debug_thread = boost::thread(ThreadPushDEBUGRoutine, log, debug_str);
    boost::thread info_thread = boost::thread(ThreadPushINFORoutine, log, info_str);
    boost::thread warning_thread = boost::thread(ThreadPushWARNINGRoutine, log, warning_str);
    boost::thread error_thread = boost::thread(ThreadPushERRORRoutine, log, error_str);

    sleep(10);

    debug_thread.join();
    info_thread.join();
    warning_thread.join();
    error_thread.join();
}

void ThreadPushDEBUGRoutine(Logger *logger, std::string str)
{
    for (int i = 0; i < 10; ++i)
    {
        logger->Log(Logger::DEBUG, str);
        sleep(1);
    }
}

void ThreadPushINFORoutine(Logger *logger, std::string str)
{
    for (int i = 0; i < 10; ++i)
    {
        logger->Log(Logger::INFO, str);
        sleep(1);
    }
}

void ThreadPushWARNINGRoutine(Logger *logger, std::string str)
{
    for (int i = 0; i < 10; ++i)
    {
        logger->Log(Logger::WARNING, str);
        sleep(1);
    }
}

void ThreadPushERRORRoutine(Logger *logger, std::string str)
{
    for (int i = 0; i < 10; ++i)
    {
        logger->Log(Logger::ERROR, str);
        sleep(1);
    }
}
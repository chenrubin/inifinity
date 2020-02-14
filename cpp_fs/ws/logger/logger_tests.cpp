#include <iostream>
#include <cstring> 
#include <fstream>
#include <sstream>

#include "logger.hpp"

using namespace ilrd;

int main()
{
	Logger log(Logger::INFO);

	std::string debug_str = "This is debug str\n";
	std::string info_str = "This is info str\n";
	std::string warning_str = "This is warning str\n";
	std::string error_str = "This is error str\n";
	
	log.Log(Logger::DEBUG, debug_str);
	log.Log(Logger::INFO, info_str);
	log.Log(Logger::WARNING, warning_str);
	log.Log(Logger::ERROR, error_str);
	log.SetOutputSeverity(Logger::ERROR);
	log.Log(Logger::DEBUG, debug_str);
	log.Log(Logger::INFO, info_str);
	log.Log(Logger::WARNING, warning_str);
	log.Log(Logger::ERROR, error_str);
	
	std::ofstream log_file;
	log_file.open("f_logger.txt", std::ofstream::out | std::ofstream::app);
	log.SetOutputSeverity(Logger::INFO);
	log.SetOutput(&log_file);
	log.Log(Logger::DEBUG, debug_str);
	log.Log(Logger::INFO, info_str);
	log.Log(Logger::WARNING, warning_str);
	log.Log(Logger::ERROR, error_str);
	log_file.close();

	std::ostringstream log_str(std::ostringstream::ate);
	log.SetOutputSeverity(Logger::DEBUG);
	log.SetOutput(&log_str);
	log.Log(Logger::DEBUG, debug_str);
	std::cout << log_str.str();
	log.Log(Logger::INFO, info_str);
	std::cout << log_str.str();
	log.Log(Logger::WARNING, warning_str);
	std::cout << log_str.str();
	log.Log(Logger::ERROR, error_str);
	std::cout << log_str.str();	
}


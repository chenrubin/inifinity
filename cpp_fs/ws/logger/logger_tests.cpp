#include <iostream>
#include <cstring> 

#include "logger.hpp"

using namespace ilrd;

int main()
{
	Logger log(Logger::INFO);

	std::string debug_str = "This is debug str";
	std::string info_str = "This is info str";
	std::string warning_str = "This is warning str";
	std::string error_str = "This is error str";
	
	log.Log(Logger::DEBUG, debug_str);
	log.Log(Logger::INFO, debug_str);
	log.Log(Logger::WARNING, debug_str);
	log.Log(Logger::ERROR, debug_str);
}

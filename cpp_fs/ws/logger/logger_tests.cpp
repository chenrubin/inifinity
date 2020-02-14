#include <iostream>
#include <cstring> 

#include "logger.hpp"

using namespace ilrd;

int main()
{
	Logger log(INFO);

	string debug_str = "This is debug str";
	string info_str = "This is info str";
	string warning_str = "This is warning str";
	string error_str = "This is error str";
	
	Log(DEBUG, debug_str);
	Log(INFO, debug_str);
	Log(WARNING, debug_str);
	Log(ERROR, debug_str);
}

explicit Logger(Severity initialSeverity = WARNING);
void Log(Severity msgSeverity, const char *msg);
void Log(Severity msgSeverity, const std::string &msg);
void SetOutputSeverity(Severity outputSeverity);
void SetOutput(std::ostream &output);


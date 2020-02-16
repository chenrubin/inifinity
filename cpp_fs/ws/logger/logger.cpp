/*******************************\
*		Author: Chen			*
*		Reviewer: Sasha			*
*		Logger					*
*		16/2/2020				*
*								*
********************************/

#include <iostream> /* std::cout */

#include "logger.hpp"

namespace ilrd
{

Logger::Logger(Severity initialSeverity)
	: m_outputSeverity(initialSeverity)
	, m_output(&std::cerr)
{
#ifndef NDEBUG	
	std::cout << "Ctor of logger" << std::endl;
#endif	
}

void Logger::Log(Severity msgSeverity, const char *msg)
{
	Log(msgSeverity, static_cast<const std::string>(msg));
}

void Logger::Log(Severity msgSeverity, const std::string &msg)
{
	if ((msgSeverity) >= (m_outputSeverity))	
	{
		*m_output << msg;
	}	
}

void Logger::SetOutputSeverity(Severity outputSeverity)
{
	m_outputSeverity = outputSeverity;
}

void Logger::SetOutput(std::ostream &output)
{
	m_output = &output;
}

} // end of namespace ilrd

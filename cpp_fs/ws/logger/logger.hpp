#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__
#include <iosfwd>

namespace ilrd
{

class Logger
{
public:
	enum Severity
	{
		DEBUG,
		INFO,
		WARNING,
		ERROR
	};
	// Ctor of the class
	explicit Logger(Severity initialSeverity = WARNING);
	
	// take a message with a given severity and either put it in output
	// or not , depending on severity rules
	void Log(Severity msgSeverity, const char *msg);
	void Log(Severity msgSeverity, const std::string &msg);
	
	// set severity so we can use it when deciding whether or not
	// to log the meassage depending on severity rules 
	void SetOutputSeverity(Severity outputSeverity);
	
	// detemine output of log (file, cerr ...)
	void SetOutput(std::ostream *output);
	
private:
	Severity m_outputSeverity;
	std::ostream *m_output;
};

} // end of namespace ilrd
#endif

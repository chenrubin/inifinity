#include <iostream>
#include <cstring> 
#include <fstream>
#include <sstream>
#include <cstdio> /* EOF */

#include "logger.hpp"
#include "MyUtils.hpp"

using namespace ilrd;

bool IsSameContent(std::ofstream &log_file1, std::ofstream &log_file2);

int main()
{
	Logger log(Logger::INFO);

	char* debug_str = "This is debug str\n";
	std::string info_str = "This is info str\n";
	char* warning_str = "This is warning str\n";
	std::string error_str = "This is error str\n";
	std::string exp_str1 = "This is debug str\n";
	std::string exp_str2 = "This is debug str\nThis is info str\n";
	std::string exp_str3 = "This is debug str\nThis is info str\nThis is warning str\n";
	std::string exp_str4 = "This is debug str\nThis is info str\nThis is warning str\nThis is error str\n";
	std::ofstream exp_log_file;
	exp_log_file.open("f_logger.txt", std::ofstream::out | std::ofstream::app);
	exp_log_file << "This is warning str" << std::endl << "This5 is error str" << std::endl;
	
	log.Log(Logger::DEBUG, debug_str);
	log.Log(Logger::INFO, info_str);
	log.Log(Logger::WARNING, warning_str);
	log.Log(Logger::ERROR, error_str);
	log.SetOutputSeverity(Logger::WARNING);
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
	PRINTTESTRESULTS("file_logger",1, true == IsSameContent(log_file, exp_log_file));
	log_file.close();
	exp_log_file.close();

	std::ostringstream log_str(std::ostringstream::ate);
	log.SetOutputSeverity(Logger::DEBUG);
	log.SetOutput(&log_str);
	log.Log(Logger::DEBUG, debug_str);
	PRINTTESTRESULTS("stringStream",1, 0 == log_str.str().compare(exp_str1));
	log.Log(Logger::INFO, info_str);
	PRINTTESTRESULTS("stringStream",2, 0 == log_str.str().compare(exp_str2));
	log.Log(Logger::WARNING, warning_str);
	PRINTTESTRESULTS("stringStream",3, 0 == log_str.str().compare(exp_str3));
	log.Log(Logger::ERROR, error_str);
	PRINTTESTRESULTS("stringStream",4, 0 == log_str.str().compare(exp_str4));	
}

bool IsSameContent(std::ofstream &log_file1, std::ofstream &log_file2)
{
	log_file1.seekp(log_file1.beg);
	log_file2.seekp(log_file2.beg);
	std::streambuf* file1_buf = log_file1.rdbuf();
	std::streambuf* file2_buf = log_file2.rdbuf();
	
	char c_file1 = file1_buf->sbumpc();
	char c_file2 = file2_buf->sbumpc();
	
	while (c_file1 != EOF)
	{
		std::cout << "inside while" << std::endl;
		if (c_file1 != c_file2)
		{
			return false;
		}
		
		c_file1 = file1_buf->sbumpc();
		c_file2 = file2_buf->sbumpc();
	}
	
	if (c_file1 == c_file2)
	{
		return true;
	}
	
	if (log_file1.eof() && log_file2.eof())
	{
		return true;
	}
	return false;
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


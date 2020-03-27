#ifndef ILRD_MYUTILS_H
#define ILRD_MYUTILS_H

#include <iostream> /* cout */
#include <stdexcept> /* std::runtime_error*/
#include <stdio.h> 	/* perror */	

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PRINTTESTRESULTS(func,num, res) \
(printf("%-55s: Test %d %s\n" KNRM,func, num, (int)((res) == 1) ?\
KGRN "\t\tpassed" : KRED "\t\tfailed"))
namespace ilrd
{
class Uncopyable 
{
protected:
	Uncopyable() {}
	~Uncopyable() {}
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};

inline void HandleErrorIfExists(int res, std::string str) 
{
	if (-1 == res)
	{
		perror(str.c_str());
        throw std::runtime_error(str);
	}
}
}
#endif

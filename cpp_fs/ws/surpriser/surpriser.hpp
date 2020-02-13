/***********************\
*	Author: Chen		*	
*	Reviewer: Hana		*
*	Surpriser			*
*	13/2/2020			*
************************/

#ifndef __SURPRISER_HPP__
#define __SURPRISER_HPP__

#include <MyUtils.hpp> /* Uncopyable */

namespace ilrd
{
class Surpriser: public Uncopyable
{
public:
	virtual ~Surpriser();
	virtual void SurpriseMe() = 0;
	static Surpriser* GetRandomSurprise();
	
private:
	static Surpriser *OutputSurprise();
	static Surpriser *SystemSurprise();
	static Surpriser *FileSurprise();	
};

class Output_d: public Surpriser
{
public:
	void SurpriseMe();
};

class System_d: public Surpriser
{
public:
	void SurpriseMe();
};

class File_d: public Surpriser
{
public:
	void SurpriseMe();
};
}// end of namespace ilrd
#endif // __SURPRISER_HPP__

/***********************\
*	Author: Chen		*	
*	Reviewer: Hana		*
*	Surpriser			*
*	13/2/2020			*
************************/

#include <iostream> /* cout */
#include <fstream> /* ofstream */
#include <ctime> /* time */
#include <cstdlib> /* rand */

#include "surpriser.hpp"
#include "MyUtils.hpp"

namespace ilrd
{

typedef Surpriser *(*surp_function)();

Surpriser::~Surpriser()
{}

void Output_d::SurpriseMe()
{
	std::cout << "Surprise!" << std::endl;
}

void System_d::SurpriseMe()
{
	system("echo -e '\\a'");
}

void File_d::SurpriseMe()
{
	std::ofstream fp;
	std::cout << "open file" << std::endl;
	fp.open("nothing_here.txt");
	fp.close();
}

Surpriser *Surpriser::GetRandomSurprise()
{	
	surp_function LUT[] = {OutputSurprise, SystemSurprise, FileSurprise};
	srand(time(NULL));
	size_t length = (sizeof(LUT) / sizeof(LUT[0]));
	int rand_value = rand() % length;
	
	return LUT[rand_value]();
}

/* LUT functions */
Surpriser *Surpriser::OutputSurprise()
{
	return (new Output_d);
}

Surpriser *Surpriser::SystemSurprise()
{
	return (new System_d);
}

Surpriser *Surpriser::FileSurprise()
{
	return (new File_d);
}
}// end of namespace ilrd

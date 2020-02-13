#include <iostream> /* cout, cin */
#include <unistd.h>

#include "surpriser.hpp"

using namespace ilrd;

int main()
{
	for (int i = 0; i < 30; i++)
	{
		sleep(1);
		Surpriser *todaysSurprise = Surpriser::GetRandomSurprise();
	
		todaysSurprise->SurpriseMe();
	
		delete todaysSurprise;
	}
	
	return 0;
}

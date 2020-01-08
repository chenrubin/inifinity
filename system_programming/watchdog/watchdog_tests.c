#include <stdio.h> /* printf */

#include "watchdog.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */



int main(int argc, char* argv[])
{
	UNUSED(argc);
	
	WatchDog(argv[2], &argv[1]);

	return 0;
}

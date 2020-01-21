#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>         
#include <sys/stat.h>     
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h>
#include <errno.h>

#include "watcher.h"
#include "MyUtils.h"

int main(int argc, char* argv[])
{
	int interval = 1;
	int dead_time = 5;
	int i = 0;
	
	int status = MMI(argv, interval, dead_time);
	
	printf("MMI status = %d\n", status);
	
	for (i = 0; i < 30; ++i)
	{
		printf("i = %d\n", i);
		sleep(1);
	}
	printf("!!!!!!!!!!!!!!!!!! DNR !!!!!!!!!!!!!!!!\n");
	DNR();
	
/*	printf("!!!!!!!!!!!!!!		KILL APP !!!!!!!!!!!!!!!!!!!!!!!!!1\n");
	raise(SIGKILL);
*/	
	for (i = 0; i < 30; ++i)
	{
		printf("after DNR i = %d\n", i);
		sleep(1);
	}

	return 0;
}

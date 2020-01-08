/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		watchdog					  *
*		7/11/2019					  *
*									  *
************************************/
#include <sys/types.h> /* pid_t */
#include <unistd.h>	   /* fork */
#include <sys/wait.h>  /* wait */
#include <stdio.h>  /* printf */
#include <string.h>  /* printf strtok */
#include <stdlib.h>  /* system */

#include "watchdog.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

static int WatchDogForkIMP(char *program, char **arg_list);
static int WatchDogSystemIMP(char *command);

int WatchDog(char *program, char **arg_list)
{
	int res = strcmp(arg_list[0], "fork");
	char str[50] = {0};
	printf("res = %d\n", res);
	if (0 == res)
	{
		printf("inside fork if\n");
		return (WatchDogForkIMP(program, &arg_list[1]));
	}
	else
	{
		printf("inside system if\n");
		
		while (NULL != *(arg_list + 1))
		{
			strcat(str, *(arg_list + 1));
			strcat(str, " ");
			++arg_list;
		}
		
		printf("str = %s\n", str);

		return (WatchDogSystemIMP(str));
	}
}

static int WatchDogForkIMP(char *program, char **arg_list)
{
	pid_t pid = 0;
	int status = 0;
	while (/*WIFEXITED(status)*/1)
	{
		sleep(1);
		pid = fork();
		if (pid > 0) /*parent*/
		{
			printf("inside /*parent*/\n");
			wait(&status);
		}
		else if (0 == pid) /*child*/
		{
			printf("inside /*child*/\n");
			printf("arg_list = %s\n", *arg_list);
			printf("program = %s\n", program);
			if (-1 == execvp(program, arg_list))
			{
				printf("exec failed\n");
			}
		}
		else
		{
			printf("fork failed\n");
		}
	}	
	
	return status;
}

static int WatchDogSystemIMP(char *command)
{
	printf("list = %s\n", command);
	while (1)
	{
		system(command);
	}
}

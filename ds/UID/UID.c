/************************************
*		Author: ChenR				  *
*		Reviewer: Tamir				  *
*		UID							  *
*		7/11/2019					  *
*									  *
************************************/
#include <time.h>
#include <unistd.h>

#include "uid.h"
#include "MyUtils.h" /* MAX2,MIN2 and print results macro*/

const ilrd_uid_t BAD_UID = {0, -1, 0};

ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t new_uid = {0};
	static size_t counter = 0;
	
	new_uid.pid = getpid();
	new_uid.time = time(NULL);
	new_uid.counter = counter;
	++counter;
	
	return new_uid;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	if (-1 == uid1.time || -1 == uid2.time)
	{
		return -1;
	}	
	else if (uid1.pid == uid2.pid && 
			 uid1.time == uid2.time &&
			 uid1.counter == uid2.counter)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int UIDIsBad(ilrd_uid_t uid)
{
	return (uid.time == BAD_UID.time);
}

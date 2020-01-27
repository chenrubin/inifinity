/************************************
*		Author: ChenR				  *
*		Reviewer: Tamir				  *
*		UID							  *
*		17/11/2019					  *
*									  *
************************************/
#include <time.h> /* time */
#include <unistd.h> /* getpid */

#include "uid.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 and print results macro*/

#ifdef THREAD_SAFE
int flag_g = 0;
#endif

const ilrd_uid_t BAD_UID = {0, -1, 0};

ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t new_uid = {0};
	static size_t counter = 0;
	
	new_uid.pid = getpid();
	new_uid.time = time(NULL);
	
#ifdef THREAD_SAFE 	
	while(__sync_lock_test_and_set(&flag_g, 1))
	{}
	__sync_lock_test_and_set(&(new_uid.counter), counter);
	__sync_add_and_fetch(&counter, 1);
	__sync_lock_release(&flag_g);
#else
	new_uid.counter = counter;
	++counter;
#endif	
	
	return new_uid;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	int result = 0;
	
	if (1 == UIDIsBad(uid1) || 1 == UIDIsBad(uid2))
	{
		result = -1;
	}	
	else if (uid1.pid == uid2.pid && 
			 uid1.time == uid2.time &&
			 uid1.counter == uid2.counter)
	{
		result = 1;
	}
	else
	{
		result = 0;
	}
	
	return result;
}

int UIDIsBad(ilrd_uid_t uid)
{
	return (uid.time == BAD_UID.time);
}

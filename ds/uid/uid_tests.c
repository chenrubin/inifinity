#include <stdio.h> /* printf */
#include <unistd.h> /* sleep */

#include "uid.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

void TestCreate();
void TestIsSame();

int main()
{
	TestCreate();
	TestIsSame();

	return 0;
}

void TestCreate()
{
	ilrd_uid_t new_uid1 = {0};
	ilrd_uid_t new_uid2 = {0};
	ilrd_uid_t new_uid3 = {0};
	
	new_uid1 = UIDCreate();
	sleep(2);
	new_uid2 = UIDCreate();
	sleep(2);
	new_uid3 = UIDCreate();
	sleep(2);
	
	PRINTTESTRESULTS("Create", 1, new_uid1.counter == 0);
	PRINTTESTRESULTS("Create", 2, new_uid2.counter == 1);	
	PRINTTESTRESULTS("Create", 3, new_uid3.counter == 2);
	PRINTTESTRESULTS("Create", 4, new_uid1.time == new_uid2.time - 2);
	PRINTTESTRESULTS("Create", 5, new_uid2.time == new_uid3.time - 2);	
	
	printf("new_uid1 = %d, %d, %ld\n", 
	new_uid1.pid, new_uid1.time, new_uid1.counter);
	printf("new_uid2 = %d, %d, %ld\n", 
	new_uid2.pid, new_uid2.time, new_uid2.counter);
	printf("new_uid3 = %d, %d, %ld\n", 
	new_uid3.pid, new_uid3.time, new_uid3.counter);
}

void TestIsSame()
{
	ilrd_uid_t new_uid1 = {0, 5 ,6};
	ilrd_uid_t new_uid2 = {0, 5 ,6};
	ilrd_uid_t new_uid3 = {0, 0 ,6};
	ilrd_uid_t new_uid4 = {1, 5 ,6};
	ilrd_uid_t new_uid5 = {0, 5 ,7};
	ilrd_uid_t new_uid6 = {0, -1 ,6};
	
	PRINTTESTRESULTS("IsSame", 1, 1 == UIDIsSame(new_uid1, new_uid2));
	PRINTTESTRESULTS("IsSame", 2, 0 == UIDIsSame(new_uid1, new_uid3));
	PRINTTESTRESULTS("IsSame", 3, 0 == UIDIsSame(new_uid1, new_uid4));
	PRINTTESTRESULTS("IsSame", 4, 0 == UIDIsSame(new_uid1, new_uid5));
	PRINTTESTRESULTS("IsSame", 4, -1 == UIDIsSame(new_uid1, new_uid6));
	
	PRINTTESTRESULTS("IsBad", 4, 0 == UIDIsBad(new_uid1));
	PRINTTESTRESULTS("IsBad", 4, 1 == UIDIsBad(new_uid6));
	
}

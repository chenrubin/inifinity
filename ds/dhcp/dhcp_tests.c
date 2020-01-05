#include <stdio.h> /* printf */
#include <math.h> /* pow */

#include "dhcp.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

void TestCreateDestroy();
void TestInsert();

#define MASK1 24
#define MASK2 30

int main()
{
	TestCreateDestroy();
	TestInsert();

	return 0;
}

void TestCreateDestroy()
{
	unsigned char ip1[] = {192,168,0,1};
	dhcp_t *new_dhcp = DHCPCreate(ip1, MASK1);
	
	DHCPDestroy(new_dhcp);
	
	printf("Create dhcp\n");
	printf("Destroy dhcp\n");
	printf("\n\n");
}

void TestInsert()
{
	unsigned char ip1[] = {192,168,0,1};
	dhcp_t *new_dhcp = DHCPCreate(ip1, MASK2);
	size_t res = pow(2, 32 - MASK2);
	
	printf("expected = %ld, actual = %ld\n", res, DHCPCountFree(new_dhcp));	
	PRINTTESTRESULTS("TestInsert_CountFree",1, res == DHCPCountFree(new_dhcp));
	
	
	
	DHCPDestroy(new_dhcp);
	
	printf("Create dhcp\n");
	printf("Destroy dhcp\n");
	printf("\n\n");
}

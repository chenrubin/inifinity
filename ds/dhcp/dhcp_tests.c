#include <stdio.h> /* printf */
#include <math.h> /* pow */
#include <string.h> /* memcmp */

#include "dhcp.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define MASK1 24
#define MASK2 27

void TestCreateDestroy();
void TestAlloc();
void TestFreeIP();


int main()
{
	TestCreateDestroy();
	TestAlloc();
	TestFreeIP();	

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

void TestAlloc()
{
	unsigned char ip1[] = {192,168,0,1};
	dhcp_t *new_dhcp = DHCPCreate(ip1, MASK2);
	size_t res = pow(2, 32 - MASK2) - 3;
	unsigned char result_ip[4] = {0};
	unsigned char valid_addr1[] = {192,168,0,15};
	unsigned char valid_addr2[] = {192,168,0,12};
	unsigned char valid_addr3[] = {192,168,0,3};
	unsigned char valid_addr4[] = {192,168,0,10};
	unsigned char invalid_addr1[] = {192,168,255,13};
	
	PRINTTESTRESULTS("TestAlloc_CountFree",1, res == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestAlloc_Alloc",2, 0 == DHCPAllocIP(new_dhcp, valid_addr1, result_ip));
	PRINTTESTRESULTS("TestAlloc_check_result_ip",3, 0 == memcmp(result_ip, valid_addr1, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestAlloc_CountFree",4, res - 1 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestAlloc_Alloc",5, 0 == DHCPAllocIP(new_dhcp, valid_addr2, result_ip));
	PRINTTESTRESULTS("TestAlloc_check_result_ip",6, 0 == memcmp(result_ip, valid_addr2, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestAlloc_CountFree",7, res - 2 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestAlloc_Alloc",8, 0 == DHCPAllocIP(new_dhcp, valid_addr3, result_ip));
	PRINTTESTRESULTS("TestAlloc_check_result_ip",9, 0 == memcmp(result_ip, valid_addr3, IP_SIZE * sizeof(unsigned char)));	
	
	PRINTTESTRESULTS("TestAlloc_CountFree",10, res - 3 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestAlloc_Alloc",11, 0 == DHCPAllocIP(new_dhcp, valid_addr4, result_ip));
	PRINTTESTRESULTS("TestAlloc_check_result_ip",12, 0 == memcmp(result_ip, valid_addr4, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestAlloc_CountFree",13, res - 4 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestAlloc_Alloc",14, 2 == DHCPAllocIP(new_dhcp, invalid_addr1, result_ip));
	PRINTTESTRESULTS("TestAlloc_CountFree",15, res - 4 == DHCPCountFree(new_dhcp));
	
	DHCPDestroy(new_dhcp);
	
	printf("Create dhcp\n");
	printf("Destroy dhcp\n");
	printf("\n\n");
}

void TestFreeIP()
{
	unsigned char ip1[] = {192,168,0,1};
	dhcp_t *new_dhcp = DHCPCreate(ip1, MASK1);
	size_t res = pow(2, 32 - MASK1) - 3;
	unsigned char result_ip[4] = {0};
	unsigned char valid_addr1[] = {192,168,0,15};
	unsigned char valid_addr2[] = {192,168,0,12};
	unsigned char valid_addr3[] = {192,168,0,3};
	unsigned char valid_addr4[] = {192,168,0,10};
	unsigned char valid_addr5[] = {192,168,0,1};
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",1, res == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Alloc",2, 0 == DHCPAllocIP(new_dhcp, valid_addr1, result_ip));
	PRINTTESTRESULTS("TestFreeIP_check_result_ip",3, 0 == memcmp(result_ip, valid_addr1, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",4, res - 1 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Alloc",5, 0 == DHCPAllocIP(new_dhcp, valid_addr2, result_ip));
	PRINTTESTRESULTS("TestFreeIP_check_result_ip",6, 0 == memcmp(result_ip, valid_addr2, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",7, res - 2 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Alloc",8, 0 == DHCPAllocIP(new_dhcp, valid_addr3, result_ip));
	PRINTTESTRESULTS("TestFreeIP_check_result_ip",9, 0 == memcmp(result_ip, valid_addr3, IP_SIZE * sizeof(unsigned char)));	
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",10, res - 3 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Alloc",11, 0 == DHCPAllocIP(new_dhcp, valid_addr4, result_ip));
	PRINTTESTRESULTS("TestFreeIP_check_result_ip",12, 0 == memcmp(result_ip, valid_addr4, IP_SIZE * sizeof(unsigned char)));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",13, res - 4 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Free",14, 0 == DHCPFreeIP(new_dhcp, valid_addr3));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",15, res - 3 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Free",16, 0 == DHCPFreeIP(new_dhcp, valid_addr1));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",17, res - 2 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Free",18, 1 == DHCPFreeIP(new_dhcp, valid_addr3));
	
	PRINTTESTRESULTS("TestFreeIP_CountFree",19, res - 2 == DHCPCountFree(new_dhcp));
	PRINTTESTRESULTS("TestFreeIP_Free",20, 2 == DHCPFreeIP(new_dhcp, valid_addr5));
	DHCPDestroy(new_dhcp);
	
	printf("Create dhcp\n");
	printf("Destroy dhcp\n");
	printf("\n\n");
}

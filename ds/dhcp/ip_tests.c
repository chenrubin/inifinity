#include <stdio.h> /* printf */
#include <string.h> /* memcmp */

#include "ip.h"

#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

void TestIPtoBit();
void TestBitToIp();
void TestCutSubnet();
void TestAddSubnet();

int main()
{
	TestIPtoBit();
	TestBitToIp();
	TestCutSubnet();	
	TestAddSubnet();	
	
	return 0;
}

void TestIPtoBit()
{
	IPAddress ip_arr1 = {192,168,0,1};
	IPAddress ip_arr2 = {10,11,12,13};
	BinaryIp res1 = IPtoBit(ip_arr1);
	BinaryIp res2 = IPtoBit(ip_arr2);
	
	
	PRINTTESTRESULTS("TestIPtoBit",1, 3232235521 == res1);
	PRINTTESTRESULTS("TestIPtoBit",2, 168496141 == res2);
}

void TestBitToIp()
{
	BinaryIp ip1 = 3232235521;
	BinaryIp ip2 = 168496141;
	IPAddress res1;
	IPAddress res2;
	IPAddress expres1 = {192,168,0,1};
	IPAddress expres2 = {10,11,12,13};
	BitToIp(ip1, res1);
	BitToIp(ip2, res2);
	
	
	PRINTTESTRESULTS("TestBitToIp",1, 0 == memcmp(res1, expres1, IP_SIZE * sizeof(unsigned char)));
	PRINTTESTRESULTS("TestBitToIp",2, 0 == memcmp(res2, expres2, IP_SIZE * sizeof(unsigned char)));
}

void TestCutSubnet()
{
	IPAddress ip_arr1 = {192,168,0,1};
	IPAddress ip_arr2 = {10,11,12,13};
	IPAddress expres1 = {0,8,0,1};
	IPAddress expres2 = {0,0,0,13};
	
	size_t mask1 = 11;
	size_t mask2 = 24;	
	
	CutSubnet(ip_arr1, mask1);
	CutSubnet(ip_arr2, mask2);
	
	PRINTTESTRESULTS("TestCutSubnet",1, 0 == memcmp(ip_arr1, expres1, IP_SIZE * sizeof(unsigned char)));
	PRINTTESTRESULTS("TestCutSubnet",2, 0 == memcmp(ip_arr2, expres2, IP_SIZE * sizeof(unsigned char)));
}

void TestAddSubnet()
{
	IPAddress ip_arr1 = {192,168,0,1};
	IPAddress ip_arr2 = {10,11,12,13};
	size_t mask1 = 11;
	size_t mask2 = 24;
	unsigned int added_part1 = 37545152;
	unsigned int added_part2 = 1492;
	IPAddress expres1 = {192,188,228,192};
	IPAddress expres2 = {10,11,10,212};
	
	AddSubnet(ip_arr1, mask1, added_part1, ip_arr1);
	AddSubnet(ip_arr2, mask2, added_part2, ip_arr2);
	
	PRINTTESTRESULTS("TestAddSubnet",1, 0 == memcmp(ip_arr1, expres1, IP_SIZE * sizeof(unsigned char)));
	PRINTTESTRESULTS("TestAddSubnet",2, 0 == memcmp(ip_arr2, expres2, IP_SIZE * sizeof(unsigned char)));
}

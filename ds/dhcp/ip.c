/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		dhcp							  *
*		7/11/2019					  *
*									  *
************************************/

#include "dhcp.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */ 
#define BYTE (sizeof(int) / 4)
#define BITS_IN_IP (32)

BinaryIp IPtoBit(IPAddress address)
{
	return ((address[0] << 24) +
			(address[1] << 16) + 
			(address[2] << 8) + 
			 address[3]);
}

void BitToIp(BinaryIp ip, IPAddress result_ip)
{
	int i = 0;
	unsigned int temp = 0;
	
	for (i = 0; i < IP_SIZE; ++i)
	{
		temp = ip & 0xFF;
		*(result_ip + IP_SIZE - i - 1) = temp;
		ip >>= 8; 
	}
}

void CutSubnet(IPAddress address, size_t mask)
{
	BinaryIp bin_ip = IPtoBit(address);
	BinaryIp mask_bin = (0xFFFFFFFF >> (BITS_IN_IP - mask)) << 
									   (BITS_IN_IP - mask);
	
	bin_ip = bin_ip & (~mask_bin);
	BitToIp(bin_ip, address);
}

void AddSubnet(IPAddress subnet, 
			   size_t mask, 
			   unsigned int added_part, 
			   IPAddress result)
{
	BinaryIp bin_ip = IPtoBit(subnet);
	BinaryIp mask_bin = (0xFFFFFFF >> (BITS_IN_IP - mask)) << 
									  (BITS_IN_IP - mask);
	
	bin_ip = (added_part ^ mask_bin) & bin_ip;
	BitToIp(bin_ip, result);
}

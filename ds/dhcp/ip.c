/************************************
*		Author: ChenR				  *
*		Reviewer: EyalF				  *
*		ip						  	  *
*		1/6/2020					  *
*									  *
************************************/
#include <string.h> /* memcmp */
#include "dhcp.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */ 
#define BYTE (sizeof(int) / 4)
#define BITS_IN_IP (32)

static BinaryIp ConvertMaskToBinaryIMP(size_t mask);

BinaryIp IPtoBit(const IPAddress address)
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

void AddSubnet(const IPAddress subnet, 
			   size_t mask, 
			   unsigned int added_part, 
			   IPAddress result)
{
	BinaryIp bin_ip = IPtoBit(subnet);
	BinaryIp mask_bin = ConvertMaskToBinaryIMP(mask);
	
	bin_ip = ((added_part & (~mask_bin)) | (bin_ip & mask_bin));
	BitToIp(bin_ip, result);
}

int IsValid(IPAddress subnet, const IPAddress address, size_t mask)
{
	BinaryIp subnet_bin = IPtoBit(subnet);
	BinaryIp address_bin = IPtoBit(address);
	BinaryIp mask_bin = ConvertMaskToBinaryIMP(mask);
	
	subnet_bin = subnet_bin & mask_bin;
	address_bin = address_bin & mask_bin;
	
	return (subnet_bin == address_bin);
}
/*
int IsSameAddress(const IPAddress ip1, const IPAddress ip2)
{
	return (0 == memcmp(ip1, ip2, IP_SIZE * sizeof(unsigned char)));
}
*/
static BinaryIp ConvertMaskToBinaryIMP(size_t mask)
{
	BinaryIp mask_bin = (0xFFFFFFFF >> (BITS_IN_IP - mask)) << 
									  (BITS_IN_IP - mask);
									  
	return mask_bin;								  
}

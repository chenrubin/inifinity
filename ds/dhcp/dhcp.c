/************************************
*		Author: ChenR				  *
*		Reviewer: EyalF				  *
*		dhcp						  *
*		6/1/2020					  *
*									  *
************************************/
#include <stdlib.h> /* free */
#include <assert.h> /* assert */
#include <math.h> /* pow */

#include "dhcp.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

#define SUB_TREE_FULL(x) (1 == (node -> x -> is_subtree_full))
#define BITS_IN_IP (32)
#define ANY_AVAILABLE_ADDRESS 0
#define MAX_ADDRESS_NUM(level) (pow(2,(level)))

/* convert level to unsigned int (binary) representation */
static unsigned int ConvertLevelToBinaryIMP(size_t level);

/* copy src to dst */
static void CopyArrIMP(IPAddress dst, const IPAddress src, size_t size);

/* insert 3 default addresses -> subnet, bc and gateway */
static int InsertDefaultAddressesIMP(dhcp_t *dhcp);

struct dhcp
{
	trie_t *trie;
	IPAddress subnet;
	size_t mask;	
};

dhcp_t *DHCPCreate(const IPAddress subnet, size_t mask)
{
	int status = 0;
	
	dhcp_t *new_dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));
	if (NULL == new_dhcp)
	{
		return NULL;
	}
	
	new_dhcp -> trie = TrieCreate(BITS_IN_IP - mask);
	if (NULL == (new_dhcp -> trie))
	{
		free(new_dhcp);
		new_dhcp = NULL;
	}
	
	CopyArrIMP(new_dhcp -> subnet, subnet, IP_SIZE);
	new_dhcp -> mask = mask;
	
	status = InsertDefaultAddressesIMP(new_dhcp);
	if (MALLOC_FAIL == status)
	{
		return NULL;
	}
	
	return new_dhcp;
}

void DHCPDestroy(dhcp_t *dhcp)
{
	TrieDestroy(dhcp -> trie);
	free(dhcp);
	dhcp = NULL;
}

alloc_status_t DHCPAllocIP(dhcp_t *dhcp, 
						   const IPAddress requested, 
						   IPAddress result_address)
{
	BinaryIp req_ip = IPtoBit(requested);
	BinaryIp res_ip = 0;
	int status = SUCCESS_ALLOCATED_REQUESTED;
	
	assert(dhcp);
	
	if(TrieIsFull(dhcp -> trie))
	{
		return DHCP_FULL;
	}
	if(!IsValid(dhcp -> subnet, requested, dhcp -> mask))
	{
		return INVALID_IP;
	}
	
	status = TrieInsert(dhcp -> trie, req_ip, &res_ip);
	
	switch (status)
	{
		case T_SUCCESS_ALLOCATED_REQUESTED:
			BitToIp(res_ip, result_address);
			return SUCCESS_ALLOCATED_REQUESTED;
			break;
		
		case T_REQUESTED_IP_OCCUPIED:
			status = TrieInsert(dhcp -> trie, ANY_AVAILABLE_ADDRESS, &res_ip);
			break;
			
		default:
			status = MALLOC_FAIL;	
	}
	
	return status;
}

free_status_t DHCPFreeIP(dhcp_t *dhcp, const IPAddress address_to_free)
{
	unsigned int bin_ip = 0;
	int status = SUCCESS;

	bin_ip = IPtoBit(address_to_free);

	status = TrieDeallocate(dhcp -> trie, bin_ip);
	switch (status)
	{
		case T_SUCCESS:
		return SUCCESS;
		break;
		
		case T_DOUBLE_FREE:
		return DOUBLE_FREE;
		break;
		
		default:
		return IP_NOT_FOUND;
		break;
	}
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
	size_t temp = (size_t)(MAX_ADDRESS_NUM(BITS_IN_IP - (dhcp -> mask)));
	
	return (temp - TrieCountAlloc(dhcp -> trie));
}

static int InsertDefaultAddressesIMP(dhcp_t *dhcp)
{
	unsigned int binary_level = ConvertLevelToBinaryIMP(BITS_IN_IP - 
														dhcp -> mask);
	unsigned int bc_binary_ip = binary_level;
	unsigned int subnet_binary_ip = 0;
	unsigned int gateway_binary_ip = binary_level - 1;
	IPAddress add_subnet_result = {0};
	IPAddress final_result = {0};
	int alloc_status = SUCCESS_ALLOCATED_REQUESTED;
	int return_status = 0;
	
	AddSubnet(dhcp -> subnet, 
			  dhcp -> mask, 
			  subnet_binary_ip, 
			  add_subnet_result);
	alloc_status = DHCPAllocIP(dhcp, add_subnet_result, final_result);
	if (alloc_status == MALLOC_FAIL)
	{
		return_status |= MALLOC_FAIL;
	}
	
	AddSubnet(dhcp -> subnet, 
			  dhcp -> mask, 
			  gateway_binary_ip, 
			  add_subnet_result);
	alloc_status = DHCPAllocIP(dhcp, add_subnet_result, final_result);
	if (alloc_status == MALLOC_FAIL)
	{
		return_status |= MALLOC_FAIL;
	}
	
	AddSubnet(dhcp -> subnet, 
			  dhcp -> mask, 
			  bc_binary_ip, 
			  add_subnet_result);
	alloc_status = DHCPAllocIP(dhcp, add_subnet_result, final_result);
	if (alloc_status == MALLOC_FAIL)
	{
		return_status |= MALLOC_FAIL;
	}

	return return_status;
}

static unsigned int ConvertLevelToBinaryIMP(size_t level)
{
	unsigned int level_bin = (0xFFFFFFFF << (BITS_IN_IP - level)) >> 
									  		(BITS_IN_IP - level);
									  
	return level_bin;								  
}

static void CopyArrIMP(IPAddress dst, const IPAddress src, size_t size)
{
	size_t i = 0;
	
	for (i = 0; i < size; ++i)
	{
		dst[i] = src[i];
	}
}

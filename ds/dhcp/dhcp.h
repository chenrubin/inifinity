/****************************************************************************
*                                                                           *
*                               OL79 - 02/01/19                             *
*                                                                           *
*                                 DHCP										*
*                                                                           *
*****************************************************************************/

#ifndef ILRD_DHCP_H
#define ILRD_DHCP_H

#include <stddef.h> /* size_t */

#include "trie.h" 
#include "ip.h"

typedef struct dhcp dhcp_t;

typedef enum status1
{
	DHCP_SUCCESS = 0,
	DHCP_DOUBLE_FREE = 1,
	DHCP_IP_NOT_FOUND = 2
}dhcp_free_status_t;

typedef enum status2
{
	DHCP_SUCCESS_ALLOCATED_REQUESTED = 0,
	DHCP_SUCCESS_OTHE_IP = 1,
	DHCP_INVALID_IP = 2,
	DHCP_DHCP_FULL = 3,
	DHCP_MALLOC_FAIL = 4
}dhcp_alloc_status_t;


/* Function creates a dhcp.
 * Receives a subnet and a mask.
 * mask must be lower than 32. 
 * Return Values:
 *               - returns pointer to the dhcp.
 *               - if the function fails, it returns NULL. */
dhcp_t *DHCPCreate(const IPAddress subnet, size_t mask);

/* The function destroys the dhcp and frees all memory allocations. */
void DHCPDestroy(dhcp_t *dhcp);

/*	The function recieves ip address requested and ip address result.
*	Function allocates ip address.
*	The allocated address will be saved in the result address  
*	If requested address equal 0, the result address will not be affected by it.
*	If allocation fails result address will be NULL */
dhcp_alloc_status_t DHCPAllocIP(dhcp_t *dhcp, const IPAddress requested, IPAddress result_address);

/*	Frees address to free.
*	If address does not exist return value will be IP_NOT_FOUND */
dhcp_free_status_t DHCPFreeIP(dhcp_t *dhcp, const IPAddress address_to_free);

/* Counts number of available addresses in the subnet */
size_t DHCPCountFree(const dhcp_t *dhcp);


#endif

#ifndef __ILRD_DHCP_H__
#define __ILRD_DHCP_H__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef struct dhcp dhcp_t;

/************************************Functions************************************/
/**
	@param: network_address - representation of an ip address in 32 bits (LSB is on the right. bits from the left will be trimmed).
	@param: subnet_mask - representation of an subnet mask in 32 bits (LSB is on the right. bits from the left will be trimmed).
	@note: subnet_mask must be a sequence of consecutive 1's starting at MSB (from the left) followed by consecutive 0's.
	@note: network_address parameter can be any address in the address space range (used for initialization)
	@note: preallocates 3 addresses: network address (first), broadcast address (last), server address (1 before last).
	@example:
		For subnet ID: 192.168.111.0/24 (mask: 255.255.255.0) the server will preallocate:
		* 192.168.111.0  
		* 192.168.111.254  
		* 192.168.111.255 
*/
dhcp_t* DHCPCreate(unsigned long network_address, unsigned long subnet_mask);

void DHCPDestroy(dhcp_t* dhcp);

/**
	@returns: 0 if no ip address was available or an internal error (memory allocation), else an ip_address in subnet range.
	@note: attempts to find the exact address or the smallest one after it. otherwise the smallest one available.
*/
unsigned long DHCPAllocateIp(dhcp_t* dhcp, unsigned long ip_address);

/** @note: does nothing when attempting to free an unallocated ip address */
void DHCPFreeIp(dhcp_t* dhcp, unsigned long ip_address);

/** @note: This calculates the number of free IPs available for allocation. */
size_t DHCPCountFree(dhcp_t* dhcp);

#endif /*__ILRD_DHCP_H__*/

/**
	Written By: Tom Golzman
	Date: 15/06/2025
	Reviewed By: Amir Granot
**/

/************************************includes************************************/
#include <assert.h> 	/* assert() */
#include <stdlib.h>		/* calloc(), free() */

#include "utils.h"		/* SUCCESS, FAIL, TRUE, FALSE, DEBUG_ONLY(), BAD_MEM() */
#include "bit_trie.h"	/* bit trie functions */
#include "dhcp.h"

/************************************typedef************************************/
struct dhcp {
	bit_trie_t* trie;
	unsigned long network_address;
	unsigned long subnet_mask;
};
	
/************************************Functions************************************/
dhcp_t* DHCPCreate(unsigned long network_address, unsigned long subnet_mask)
{
	dhcp_t* dhcp = NULL;
	size_t host_bits = 0;
	unsigned long mask = subnet_mask;
	unsigned long broadcast = 0;
	unsigned long server = 0;
	
	assert(subnet_mask != 0);

	/* calculate number of bits for the Host ID */
	while (0 == (mask & 1UL))
	{
		++host_bits;
		mask >>= 1;
	}

	/* allocate memory for dhcp struct */
	dhcp = (dhcp_t*)calloc(1, sizeof(dhcp_t));
	if (NULL == dhcp)
	{
		return NULL;
	}
	
	/* allocate memory for trie by num of bits for the Host ID */
	dhcp->trie = BitTrieCreate(host_bits);
	if (NULL == dhcp->trie)
	{
		free(dhcp);
		
		return NULL;
	}
	
	/* initialize struct fields */
	dhcp->network_address = network_address & subnet_mask;
	dhcp->subnet_mask = subnet_mask;
	
	/* calculate broadcast address (host bits are 1) and server address (biggest address before broadcast) */
	broadcast = network_address | (~subnet_mask);
	server = broadcast - 1;
	
	/* preaclloate special addresses: network, broadcast, server */
	if (BTrieGet(dhcp->trie, network_address) != network_address || 
		BTrieGet(dhcp->trie, broadcast) != broadcast ||
		BTrieGet(dhcp->trie, server) != server)
	{
		BTrieDestroy(dhcp->trie);
		free(dhcp);
		
		return NULL;
	}
	
	return dhcp;
}

void DHCPDestroy(dhcp_t* dhcp)
{
	assert(NULL != dhcp);
	
	BTrieDestroy(dhcp->trie);
	
	DEBUG_ONLY(
		dhcp->trie = BAD_MEM(bit_trie_t*);
		dhcp->network_address = 0;
		dhcp->subnet_mask = 0;
	);
	
	free(dhcp);
	dhcp = NULL;
}

unsigned long DHCPAllocateIp(dhcp_t* dhcp, unsigned long ip_address)
{
	unsigned long host_mask = 0;
	unsigned long host_id = 0;
	unsigned long allocated_host_id = 0;
	
	assert(NULL != dhcp);

	/* calcualte host_mask and host ID to allocate */
	host_mask = ~(dhcp->subnet_mask);
	host_id = ip_address & host_mask;
	
	/* try to alloacte the requested host ID */
	allocated_host_id = BTrieGet(dhcp->trie, host_id);
	if (0 != allocated_host_id)
	{
		/* return the network address with the allocated host id */
		return dhcp->network_address | allocated_host_id;
	}
	
	/* if alloacation failed - return 0 */
	return 0;
}

void DHCPFreeIp(dhcp_t* dhcp, unsigned long ip_address)
{
	unsigned long host_mask = 0;
	unsigned long host_id_to_free = 0;
	
	assert(NULL != dhcp);
	
	/* calcualte host_mask and host ID to free */
	host_mask = ~(dhcp->subnet_mask);
	host_id_to_free = ip_address & host_mask;
	
	BTrieFree(dhcp->trie, host_id_to_free);
}

size_t DHCPCountFree(dhcp_t* dhcp)
{
	assert(NULL != dhcp);

	return BTrieCountFree(dhcp->trie);	
}

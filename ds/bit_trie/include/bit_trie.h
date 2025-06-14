#ifndef __ILRD_BIT_TRIE_H__
#define __ILRD_BIT_TRIE_H__

/************************************includes************************************/
#include <stddef.h> /* size_t */

/************************************typedef************************************/
typedef struct bit_trie bit_trie_t;

/************************************define************************************/

/************************************Functions************************************/
bit_trie_t* BitTrieCreate(size_t num_bits);

void BTrieDestroy(bit_trie_t*);

void BTrieClear(bit_trie_t*);

/** 
	@input: trie_value = 0 means get any
	@output: 0 means fail (no more addresses or malloc failed), else - a generated sequence. 
*/
unsigned long BTrieGet(bit_trie_t*, unsigned long trie_value);

void BTrieFree(bit_trie_t*, unsigned long trie_value);

/** @return: Number of leaves a.k.a number of different sequences **/
size_t BTrieCountFree(const bit_trie_t*);

#endif /*__ILRD_BIT_TRIE_H__*/

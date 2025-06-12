#ifndef __ILRD_BIT_TRIE_H__
#define __ILRD_BIT_TRIE_H__

/************************************includes************************************/

/************************************typedef************************************/
typedef struct bit_trie bit_trie_t;

/************************************define************************************/

/************************************Functions************************************/
bit_trie_t* BTrieCreate();
void Destroy(bit_trie_t*);
int Insert(bit_trie_t*, const char* bits);
void Remove(bit_trie_t*, const char* bits_to_remove);
int Find(const bit_trie_t*, const char* bits_to_find);

#endif /*__ILRD_BIT_TRIE_H__*/

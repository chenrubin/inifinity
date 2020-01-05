#include <stdio.h> /* printf */

/*#include "dhcp.h"*/
#include "trie.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

void TestCreateDestroy();
void TestInsertTrie();
void TestDeallocate();
/*
display trie->node->children[0]->children[0]->children[0]->is_subtree_full
display trie->node->children[0]->children[0]->children[1]->is_subtree_full
display trie->node->children[0]->children[1]->children[0]->is_subtree_full
display trie->node->children[0]->children[1]->children[1]->is_subtree_full
display trie->node->children[1]->children[0]->children[0]->is_subtree_full
display trie->node->children[1]->children[0]->children[1]->is_subtree_full
display trie->node->children[1]->children[1]->children[0]->is_subtree_full
display trie->node->children[1]->children[1]->children[1]->is_subtree_full
*/
int main()
{
	TestCreateDestroy();
	TestInsertTrie();
	TestDeallocate();	

	return 0;
}

void TestCreateDestroy()
{
	size_t level = 20;
	trie_t *new_trie = TrieCreate(level);
	
	TrieDestroy(new_trie);
	printf("create trie\n");
	printf("Destroy trie\n\n");
}

void TestInsertTrie()
{
	size_t level = 3;
	trie_t *new_trie = TrieCreate(level);
	unsigned int result_ip = 0;
	
	PRINTTESTRESULTS("TestInsertTrie",1, 0 == TrieInsert(new_trie, 3232235521/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsertTrie",2, 0 == TrieInsert(new_trie, 3232235522/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsertTrie",3, 0 == TrieInsert(new_trie, 3232235523/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsertTrie",4, 2 == TrieInsert(new_trie, 3232235523/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsertTrie",5, 0 == TrieInsert(new_trie, 3232235524/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsertTrie",6, 0 == TrieInsert(new_trie, 3232235526/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsertTrie",7, 0 == TrieInsert(new_trie, 3232235527/*192.168.0.1*/, &result_ip));
	
	TrieDestroy(new_trie);
}

void TestDeallocate()
{
	size_t level = 3;
	trie_t *new_trie = TrieCreate(level);
	unsigned int result_ip = 0;
	
	PRINTTESTRESULTS("TestDeallocate_allocate",1, 0 == TrieInsert(new_trie, 3232235521/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestDeallocate_allocate",2, 0 == TrieInsert(new_trie, 3232235522/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestDeallocate_allocate",3, 0 == TrieInsert(new_trie, 3232235523/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestDeallocate_deallocate",4, 0 == TrieDeallocate(new_trie, 3232235521));
	PRINTTESTRESULTS("TestDeallocate_deallocate",5, 1 == TrieDeallocate(new_trie, 3232235521));
	PRINTTESTRESULTS("TestDeallocate_deallocate",6, 1 == TrieDeallocate(new_trie, 3232235525));	
	
	TrieDestroy(new_trie);
}	

#include <stdio.h> /* printf */

#include "dhcp.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

void TestInsertTrie();

int main()
{
	TestInsertTrie();

	return 0;
}

void TestInsertTrie()
{
	size_t level = 3;
	trie_t *new_trie = TrieCreate(level);
	unsigned int result_ip = 0;
	
	PRINTTESTRESULTS("TestInsert_trie",1, 0 == TrieInsert(new_trie, 3232235521/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsert_trie",2, 0 == TrieInsert(new_trie, 3232235522/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsert_trie",3, 0 == TrieInsert(new_trie, 3232235523/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsert_trie",4, 1 == TrieInsert(new_trie, 3232235523/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsert_trie",5, 1 == TrieInsert(new_trie, 3232235524/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsert_trie",6, 3 == TrieInsert(new_trie, 3232235526/*192.168.0.1*/, &result_ip));
	PRINTTESTRESULTS("TestInsert_trie",7, 3 == TrieInsert(new_trie, 3232235527/*192.168.0.1*/, &result_ip));
}
/*SUCCESS_ALLOCATED_REQUSTED = 0,
	SUCCESS_ALLOCATED_AVAILABLE = 1,
	MALLOC_FAIL = 2,
	TRIE_FULL = 3*/

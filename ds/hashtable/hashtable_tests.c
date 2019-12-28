#include <stdio.h> /* printf */
#include <string.h> /* strcmp */
#include <strings.h> /* strcasecmp */
#include <ctype.h> /* toupper */

#include "hashtable.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

void TestCreateDestory();
void TestInsert();
void TestRemove();
void TestFind();
void TestStd();

size_t MyHashFunction(const void *key);
int MyIsMatchFunction(const void *current_data, const void *data_to_compare);
int MyActionFunction(void *data, void *action_param);

int main()
{
	TestCreateDestory();
	TestInsert();
	TestRemove();	
	TestFind();
	TestStd();
	
	return 0;
}

void TestCreateDestory()
{
	size_t num_of_buckets = 26;
	hash_table_t *new_htbl = HashCreate(num_of_buckets, 
										MyHashFunction,
										MyIsMatchFunction);
	printf("Create hash table\n");
	printf("Destroy hash table\n");
	HashDestroy(new_htbl);
	printf("\n\n");
}

void TestInsert()
{
	char *a = "asulin";
	char *b = "auslin";
	char *c = "moshe";
	char *d = "My god";
	char *e = "Auto";
	char *f = "zanzibar";
	size_t num_of_buckets = 26;
	int action_param = 0;
	hash_table_t *new_htbl = HashCreate(num_of_buckets, 
										MyHashFunction,
										MyIsMatchFunction);
	PRINTTESTRESULTS("TestInsert_isempty",1, 1 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestInsert_size",2, 0 == HashSize(new_htbl));
	PRINTTESTRESULTS("TestInsert_insert",3, 0 == HashInsert(new_htbl, a));
	PRINTTESTRESULTS("TestInsert_isempty",4, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestInsert_size",5, 1 == HashSize(new_htbl));
	PRINTTESTRESULTS("TestInsert_insert",6, 0 == HashInsert(new_htbl, b));
	PRINTTESTRESULTS("TestInsert_isempty",7, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestInsert_size",8, 2 == HashSize(new_htbl));
	PRINTTESTRESULTS("TestInsert_insert",9, 0 == HashInsert(new_htbl, c));
	PRINTTESTRESULTS("TestInsert_isempty",10, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestInsert_size",11, 3 == HashSize(new_htbl));
	PRINTTESTRESULTS("TestInsert_insert",12, 0 == HashInsert(new_htbl, d));
	PRINTTESTRESULTS("TestInsert_isempty",13, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestInsert_size",14, 4 == HashSize(new_htbl));
	PRINTTESTRESULTS("TestInsert_insert",15, 0 == HashInsert(new_htbl, e));
	PRINTTESTRESULTS("TestInsert_isempty",16, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestInsert_size",17, 5 == HashSize(new_htbl));
	PRINTTESTRESULTS("TestInsert_insert",18, 0 == HashInsert(new_htbl, f));
	PRINTTESTRESULTS("TestInsert_isempty",19, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestInsert_size",20, 6 == HashSize(new_htbl));
	
	HashForEach(new_htbl, MyActionFunction, &action_param);

	HashDestroy(new_htbl);
	printf("\n\n");									
}

void TestRemove()
{
	char *a = "asulin";
	char *b = "auslin";
	char *c = "moshe";
	char *d = "My god";
	char *e = "Auto";
	char *f = "zanzibar";
	char *g = "shocker";
	size_t num_of_buckets = 26;
	int action_param = 0;
	hash_table_t *new_htbl = HashCreate(num_of_buckets, 
										MyHashFunction,
										MyIsMatchFunction);
										
	PRINTTESTRESULTS("TestRemove_insert",1, 0 == HashInsert(new_htbl, a));
	PRINTTESTRESULTS("TestRemove_insert",2, 0 == HashInsert(new_htbl, b));		
	PRINTTESTRESULTS("TestRemove_insert",3, 0 == HashInsert(new_htbl, c));
	PRINTTESTRESULTS("TestRemove_insert",4, 0 == HashInsert(new_htbl, d));
	PRINTTESTRESULTS("TestRemove_insert",5, 0 == HashInsert(new_htbl, e));
	PRINTTESTRESULTS("TestRemove_insert",6, 0 == HashInsert(new_htbl, f));
	
	HashForEach(new_htbl, MyActionFunction, &action_param);
	printf("\n");	
	PRINTTESTRESULTS("TestInsert_isempty",7, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestRemove_size",8, 6 == HashSize(new_htbl));
	printf("Remove %s\n", d);
	HashRemove(new_htbl, d);
	PRINTTESTRESULTS("TestInsert_isempty",9, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestRemove_size",10, 5 == HashSize(new_htbl));
	HashForEach(new_htbl, MyActionFunction, &action_param);
	printf("\n");
	printf("Remove %s\n", a);
	HashRemove(new_htbl, a);
	PRINTTESTRESULTS("TestInsert_isempty",11, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestRemove_size",12, 4 == HashSize(new_htbl));
	HashForEach(new_htbl, MyActionFunction, &action_param);
	printf("\n");
	printf("Remove %s\n", f);
	HashRemove(new_htbl, f);
	PRINTTESTRESULTS("TestInsert_isempty",13, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestRemove_size",14, 3 == HashSize(new_htbl));
	HashForEach(new_htbl, MyActionFunction, &action_param);
	printf("\n");
	printf("Remove %s\n", g);
	HashRemove(new_htbl, g);
	printf("Remove %s\n", b);
	HashRemove(new_htbl, b);
	PRINTTESTRESULTS("TestInsert_isempty",15, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestRemove_size",16, 2 == HashSize(new_htbl));
	HashForEach(new_htbl, MyActionFunction, &action_param);
	printf("\n");
	printf("Remove %s\n", c);
	HashRemove(new_htbl, c);
	PRINTTESTRESULTS("TestInsert_isempty",17, 0 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestRemove_size",18, 1 == HashSize(new_htbl));
	HashForEach(new_htbl, MyActionFunction, &action_param);
	printf("\n");
	printf("Remove %s\n", e);
	HashRemove(new_htbl, e);
	PRINTTESTRESULTS("TestInsert_isempty",19, 1 == HashIsEmpty(new_htbl));
	PRINTTESTRESULTS("TestRemove_size",20, 0 == HashSize(new_htbl));
	HashForEach(new_htbl, MyActionFunction, &action_param);	
	printf("\n"); 	
	HashDestroy(new_htbl);
	printf("\n\n");									
}

void TestFind()
{
	char *a = "asulin";
	char *b = "auslin";
	char *c = "moshe";
	char *d = "My god";
	char *e = "Auto";
	char *f = "zanzibar";
	char *g = "shocker";
	size_t num_of_buckets = 26;
	hash_table_t *new_htbl = HashCreate(num_of_buckets, 
										MyHashFunction,
										MyIsMatchFunction);
										
	PRINTTESTRESULTS("TestFind_insert",1, 0 == HashInsert(new_htbl, a));
	PRINTTESTRESULTS("TestFind_insert",2, 0 == HashInsert(new_htbl, b));
	PRINTTESTRESULTS("TestFind_insert",3, 0 == HashInsert(new_htbl, c));
	PRINTTESTRESULTS("TestFind_insert",4, 0 == HashInsert(new_htbl, d));
	PRINTTESTRESULTS("TestFind_insert",5, 0 == HashInsert(new_htbl, e));
	PRINTTESTRESULTS("TestFind_insert",6, 0 == HashInsert(new_htbl, f));
	
	PRINTTESTRESULTS("TestFind_find",7, 0 == strcmp(a, HashFind(new_htbl, a)));
	PRINTTESTRESULTS("TestFind_find",7, 0 == strcmp(b, HashFind(new_htbl, b)));
	PRINTTESTRESULTS("TestFind_find",7, 0 == strcmp(c, HashFind(new_htbl, c)));
	PRINTTESTRESULTS("TestFind_find",7, 0 == strcmp(d, HashFind(new_htbl, d)));
	PRINTTESTRESULTS("TestFind_find",7, 0 == strcmp(e, HashFind(new_htbl, e)));
	PRINTTESTRESULTS("TestFind_find",7, 0 == strcmp(f, HashFind(new_htbl, f)));
	PRINTTESTRESULTS("TestFind_find",7, NULL == HashFind(new_htbl, g));
	
	printf("Remove %s\n", a);
	HashRemove(new_htbl, a);
	PRINTTESTRESULTS("TestFind_find",7, NULL == HashFind(new_htbl, a));
	PRINTTESTRESULTS("TestFind_insert",6, 0 == HashInsert(new_htbl, g));
	PRINTTESTRESULTS("TestFind_find",7, 0 == strcmp(g, HashFind(new_htbl, g)));
	printf("\n");	
	
	HashDestroy(new_htbl);
	printf("\n\n");									
}

void TestStd()
{
	char *a = "asulin";
	char *b = "auslin";
	char *c = "moshe";
	char *d = "My god";
	char *e = "Auto";
	char *f = "zanzibar";
	size_t num_of_buckets = 26;
	int action_param = 0;
	hash_table_t *new_htbl = HashCreate(num_of_buckets, 
										MyHashFunction,
										MyIsMatchFunction);
	PRINTTESTRESULTS("TestStd",3, 0 == HashInsert(new_htbl, a));
	PRINTTESTRESULTS("TestStd",6, 0 == HashInsert(new_htbl, b));
	PRINTTESTRESULTS("TestStd",9, 0 == HashInsert(new_htbl, c));
	PRINTTESTRESULTS("TestStd",12, 0 == HashInsert(new_htbl, d));
	PRINTTESTRESULTS("TestStd",15, 0 == HashInsert(new_htbl, e));
	PRINTTESTRESULTS("TestStd",18, 0 == HashInsert(new_htbl, f));
	
	HashForEach(new_htbl, MyActionFunction, &action_param);
	printf("standard deviation = %f\n", HashGetStandardDeviation(new_htbl));
	
	HashDestroy(new_htbl);
	printf("\n\n");									
}

size_t MyHashFunction(const void *key)
{
	size_t index = (size_t)toupper(*(char *)key) - 'A';
	
	return index;
}

int MyIsMatchFunction(const void *current_data, const void *data_to_compare)
{
	return (0 == strcasecmp((char *)current_data, (char *)data_to_compare));
}

int MyActionFunction(void *data, void *action_param)
{
	UNUSED(action_param);
	
	printf("%s\n", (char *)data);
	
	return 0;
}

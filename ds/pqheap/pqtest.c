#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf */

#include "my_utils.h" /* macros */
#include "pqheap.h" 

int Is_Match(const void *new_data, const void *param);
int compare(const void *new_data, const void *src_data, void *param);

void TestPQCreateAndEnqueue();
void TestPQDequeue();
void TestPQPeek();
void TestPQSizeAndClear();
void TestPQErase();

void TestPQErase()
{
	int priority_param = 10;
	int x = NUM_FOR_TESTING * 1.5;
	int y = NUM_FOR_TESTING;
	int z = NUM_FOR_TESTING * 2;	
	int res = 0;
	p_queue_t *my_pq = PQCreate(&priority_param, compare);	
	
	PQEnqueue(my_pq, &y);
	PQEnqueue(my_pq, &z);
	PQEnqueue(my_pq, &x);
	
	res = *(int *)(PQErase(&x, my_pq, Is_Match));
	PRINTTESTRESULTS("PQErase", 1, (PQSize(my_pq) == 2));
	PRINTTESTRESULTS("PQErase", 2, (res == x));
	PQErase(&y, my_pq, Is_Match);
	res = (*((int *)(PQPeek(my_pq))));

 	PRINTTESTRESULTS("PQErase", 3, (res == z));
	
	
	PQDestroy(my_pq);
}
void TestPQSizeAndClear()
{
	int priority_param = 10;
	int x = NUM_FOR_TESTING * 1.5;
	int y = NUM_FOR_TESTING;

	int t = NUM_FOR_TESTING -1;

	p_queue_t *my_pq = PQCreate(&priority_param, compare);
		
	PRINTTESTRESULTS("PQSize", 1, (PQSize(my_pq) == 0));
	PQEnqueue(my_pq, &y);
	PQEnqueue(my_pq, &t);
	PQEnqueue(my_pq, &x);

	PRINTTESTRESULTS("PQSize", 2, (PQSize(my_pq) == 3));
	PQDequeue(my_pq);
	PRINTTESTRESULTS("PQSize", 3, (PQSize(my_pq) == 2));
	PQClear(my_pq);
	PRINTTESTRESULTS("PQClear", 3, (PQSize(my_pq) == 0));
	PQDestroy(my_pq);
}

void TestPQPeek()
{
	int priority_param = 10;
	int x = NUM_FOR_TESTING * 1.5;
	int y = NUM_FOR_TESTING;
	int z = NUM_FOR_TESTING * 2;
	int t = NUM_FOR_TESTING -1;
	int res = 0;
	p_queue_t *my_pq = PQCreate(&priority_param, compare);	
	
 	PRINTTESTRESULTS("PQIsEmpty", 1, (PQIsEmpty(my_pq) == 1));
	

	PQEnqueue(my_pq, &y);
	PQEnqueue(my_pq, &t);
	PQEnqueue(my_pq, &x);
	PRINTTESTRESULTS("PQIsEmpty", 2, (PQIsEmpty(my_pq) == 0));
	PRINTTESTRESULTS("PQSize", 1, (PQSize(my_pq) == 3));
	res = (*((int *)(PQPeek(my_pq))));

 	PRINTTESTRESULTS("PQPeek", 1, (res == x));
	
	PQEnqueue(my_pq, &z);
	res = (*((int *)(PQPeek(my_pq))));

 	PRINTTESTRESULTS("PQPeek", 2, (res == z));
 	
	PQDestroy(my_pq);
}
void TestPQDequeue()
{
	int priority_param = 10;
	int x = NUM_FOR_TESTING * 1.5;
	int y = NUM_FOR_TESTING;
	int z = NUM_FOR_TESTING * 2;
	int t = NUM_FOR_TESTING -1;
	int res = 0;
	p_queue_t *my_pq = PQCreate(&priority_param, compare);	
	

	PQEnqueue(my_pq, &y);
	PQEnqueue(my_pq, &t);
	PQEnqueue(my_pq, &x);
	
	res = (*((int *)(PQDequeue(my_pq))));

 	PRINTTESTRESULTS("PQDequeue", 1, (res == x));
	
	PQEnqueue(my_pq, &z);
	res = (*((int *)(PQDequeue(my_pq))));
 	PRINTTESTRESULTS("PQDequeue", 2, (res == z));
 	
	PQDestroy(my_pq);
}

void TestPQCreateAndEnqueue()
{
	int priority_param = 10;
	int x = NUM_FOR_TESTING * 1.5;
	int y = NUM_FOR_TESTING;
	int z = NUM_FOR_TESTING * 2;
	int t = NUM_FOR_TESTING - 1;
	int res = 0;
	p_queue_t *my_pq = PQCreate(&priority_param, compare);	
PQEnqueue(my_pq, &z);

	
	PQEnqueue(my_pq, &y);
	PQEnqueue(my_pq, &t);
	PQEnqueue(my_pq, &x);
	
	res = (*((int *)(PQPeek(my_pq))));

 	PRINTTESTRESULTS("PQEnqueue", 1, (res == z));
	
	PQDequeue(my_pq);
	res = (*((int *)(PQPeek(my_pq))));

 	PRINTTESTRESULTS("PQEnqueue", 2, (res == x));
	/**/
	PQDestroy(my_pq);	
}

int main()
{
	
	
	TestPQCreateAndEnqueue();/**/
	TestPQSizeAndClear();
	TestPQPeek();
	TestPQDequeue();
	TestPQErase();
	return 0;
}

int Is_Match(const void *new_data, const void *param)
{
	if ((int *)new_data == (int *)param)
	{
		return 1;
	}

	return 0;
}

int compare(const void *new_data, const void *src_data, void *param)
{
	UNUSED(param);
	
	if (*((int *)new_data) > *((int *)src_data))
	{
		return 1;
	}
	if (*((int *)new_data) < *((int *)src_data))
	{
		return -1;
	}
	
	return 0;	
}




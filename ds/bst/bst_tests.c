#include <stdio.h> /* printf */
#include <stdlib.h> /* printf */

#include "bst.h"
#include "MyUtils.h" /* MAX2,MIN2 */

void something(int **p1);

int main()
{
	int *p1 = NULL;
	
	something(&p1);
	printf("p1 = %d\n", *p1);

	return 0;
}

void something(int **p1)
{
	int *p2 = *p1;
	p2 = (int *)malloc(sizeof(int));
	
	*p2 = 5;
}

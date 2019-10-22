#include <stdio.h>

#define MAX2(a,b) a > b ? a : b
#define MAX3(a,b,c) a > b ? (a > c ? a : c) : (b > c ? b : c)

int main()
{
	printf("%d\n", MAX2(3,2));
	printf("%d\n", MAX2(2,3));
	printf("%d\n", MAX3(3,2,7));
	printf("%d\n", MAX3(7,3,2));
	printf("%d\n", MAX3(3,7,2));
	
	return 0;
} 

#include <stdio.h> /* printf */

#include "calc.h"
#include "../stack/stack.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */



int main()
{
	char sentence[] = "541+568+1";
	char sentence1[] = "5-2";
	double res;
	
	Calc(sentence, &res);
	printf("res = %f\n", res);
	
	Calc(sentence1, &res);
	printf("res = %f\n", res);

	
	return 0;
}

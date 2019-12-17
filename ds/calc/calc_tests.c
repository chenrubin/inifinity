#include <stdio.h> /* printf */

#include "calc.h"
#include "../stack/stack.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */



int main()
{
	char sentence[] = "1+2";
	double res;
	
	Calc(sentence, &res);
	
	printf("res = %f\n");
	
	return 0;
}

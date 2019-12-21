#include <stdio.h> /* printf */

#include "calc.h"
#include "../../chen/MyUtils.h" /* MAX2,MIN2 */

void TestAdd();
void TestSubtract();
void TestMultiply();
void TestDivide();
void TestAllOperations();
void TestInvalid();
void TestParentheses();
void TestPower();


int main()
{
	TestAdd();
	TestSubtract();
	TestMultiply();
	TestDivide();
	TestAllOperations();
	TestInvalid();
	TestParentheses();
	TestPower();	

	return 0;
}

void TestAdd()
{
	char *sentence1 = "12+3+4+5664+12+59";
	char *sentence2 = "12+0+1+23";
	char *sentence3 = "12";
	char *sentence4 = "2";
	double res = 0;
		
	PRINTTESTRESULTS("TestAdd_calc",1, 0 == Calc(sentence1, &res));
	PRINTTESTRESULTS("TestAdd",2, 5754 == res);
	PRINTTESTRESULTS("TestAdd_calc",3, 0 == Calc(sentence2, &res));
	PRINTTESTRESULTS("TestAdd",4, 36 == res);
	PRINTTESTRESULTS("TestAdd_calc",5, 0 == Calc(sentence3, &res));
	PRINTTESTRESULTS("TestAdd",6, 12 == res);
	PRINTTESTRESULTS("TestAdd_calc",7, 0 == Calc(sentence4, &res));
	PRINTTESTRESULTS("TestAdd",8, 2 == res);
}

void TestSubtract()
{
	char *sentence1 = "100-56-89-52-0-85";
	char *sentence2 = "15-56-85-99999";
	char *sentence3 = "1-2";
	char *sentence4 = "1-9-6-5-8-9-2-6";
	double res = 0;
		
	PRINTTESTRESULTS("TestSubtract_calc",1, 0 == Calc(sentence1, &res));
	PRINTTESTRESULTS("TestSubtract",2, -182 == res);
	PRINTTESTRESULTS("TestSubtract_calc",3, 0 == Calc(sentence2, &res));
	PRINTTESTRESULTS("TestSubtract",4, -100125 == res);
	PRINTTESTRESULTS("TestSubtract_calc",5, 0 == Calc(sentence3, &res));
	PRINTTESTRESULTS("TestSubtract",6, -1 == res);
	PRINTTESTRESULTS("TestSubtract_calc",7, 0 == Calc(sentence4, &res));
	PRINTTESTRESULTS("TestSubtract",8, -44 == res);
}

void TestMultiply()
{
	char *sentence1 = "100*95*8*85236*5";
	char *sentence2 = "2*3";
	char *sentence3 = "56*84";
	char *sentence4 = "1*2*3*4*5685";
	double res = 0;
		
	PRINTTESTRESULTS("TestMultiply_calc",1, 0 == Calc(sentence1, &res));
	PRINTTESTRESULTS("TestMultiply",2, 32389680000 == res);
	PRINTTESTRESULTS("TestMultiply_calc",3, 0 == Calc(sentence2, &res));
	PRINTTESTRESULTS("TestMultiply",4, 6 == res);
	PRINTTESTRESULTS("TestMultiply_calc",5, 0 == Calc(sentence3, &res));
	PRINTTESTRESULTS("TestMultiply",6, 4704 == res);
	PRINTTESTRESULTS("TestMultiply_calc",7, 0 == Calc(sentence4, &res));
	PRINTTESTRESULTS("TestMultiply",8, 136440 == res);
}

void TestDivide()
{
	char *sentence1 = "100/25";
	char *sentence2 = "9/100";
	char *sentence3 = "1/5";
	char *sentence4 = "64/8/16";
	double res = 0;
		
	PRINTTESTRESULTS("TestDivide_calc",1, 0 == Calc(sentence1, &res));
	PRINTTESTRESULTS("TestDivide",2, 4 == res);
	PRINTTESTRESULTS("TestDivide_calc",3, 0 == Calc(sentence2, &res));
	PRINTTESTRESULTS("TestDivide",4, 0.09 == res);
	PRINTTESTRESULTS("TestDivide_calc",5, 0 == Calc(sentence3, &res));
	PRINTTESTRESULTS("TestDivide",6, 0.2 == res);
	PRINTTESTRESULTS("TestDivide_calc",7, 0 == Calc(sentence4, &res));
	PRINTTESTRESULTS("TestDivide",8, 0.5 == res);
}

void TestAllOperations()
{
	char *sentence1 = "100*52*56/2+54*5-8/4-6";
	char *sentence2 = "32*32/32+9/3*8-4*5";
	char *sentence3 = "0.2*5-9.5*6";
	char *sentence4 = "1-1/2*2+1";
	char *sentence5 = "1-1/2^2+9*(64/(20+12))/8-1";
	char *sentence6 = "0.54+.46-0.5*4^8-(+25)";
	
	double res = 0;
		
	PRINTTESTRESULTS("TestAllOperations_calc",1, 0 == Calc(sentence1, &res));
	PRINTTESTRESULTS("TestAllOperations",2, 145862 == res);
	PRINTTESTRESULTS("TestAllOperations_calc",3, 0 == Calc(sentence2, &res));
	PRINTTESTRESULTS("TestAllOperations",4, 36 == res);
	PRINTTESTRESULTS("TestAllOperations_calc",5, 0 == Calc(sentence3, &res));
	PRINTTESTRESULTS("TestAllOperations",6, -56 == res);
	PRINTTESTRESULTS("TestAllOperations_calc",7, 0 == Calc(sentence4, &res));
	PRINTTESTRESULTS("TestAllOperations",8, 1 == res);
	PRINTTESTRESULTS("TestAllOperations_calc",9, 0 == Calc(sentence5, &res));
	PRINTTESTRESULTS("TestAllOperations",10, 2 == res);
	PRINTTESTRESULTS("TestAllOperations_calc",11, 0 == Calc(sentence6, &res));
	PRINTTESTRESULTS("TestAllOperations",12, -32792 == res);
}

void TestInvalid()
{
	char *sentence1 = "4--5";
	char *sentence2 = "5++8";
	char *sentence3 = "8**9";
	char *sentence4 = "152//5";
	char *sentence5 = "152*+5";
	char *sentence6 = "152-*5";
	char *sentence7 = "152/0";
	char *sentence8 = "(152/0";
	char *sentence9 = "(152/0))";
	
	double res = 0;
		
	PRINTTESTRESULTS("TestInvalid_calc",1, 0 == Calc(sentence1, &res));
	PRINTTESTRESULTS("TestInvalid_calc",2, 0 == Calc(sentence2, &res));
	PRINTTESTRESULTS("TestInvalid_calc",3, 2 == Calc(sentence3, &res));
	PRINTTESTRESULTS("TestInvalid_calc",4, 2 == Calc(sentence4, &res));
	PRINTTESTRESULTS("TestInvalid_calc",5, 0 == Calc(sentence5, &res));
	PRINTTESTRESULTS("TestInvalid_calc",6, 2 == Calc(sentence6, &res));
	PRINTTESTRESULTS("TestInvalid_calc",7, 1 == Calc(sentence7, &res));
	PRINTTESTRESULTS("TestInvalid_calc",8, 2 == Calc(sentence8, &res));
	PRINTTESTRESULTS("TestInvalid_calc",9, 2 == Calc(sentence9, &res));

}

void TestParentheses()
{
	char *sentence1 = "1*(2+3)";
	char *sentence2 = "1*(2+3*9+4)-(98-25-4)/2";
	char *sentence3 = "(256*65-1)+(25)+56/2-(54)";
	char *sentence4 = "(66/2*(43-55*(7*6)))";
	char *sentence5 = "5*(-2)";
	
	double res = 0;
		
	PRINTTESTRESULTS("TestParentheses_calc",1, 0 == Calc(sentence1, &res));
	PRINTTESTRESULTS("TestParentheses",2, 5 == res);
	PRINTTESTRESULTS("TestParentheses_calc",3, 0 == Calc(sentence2, &res));
	PRINTTESTRESULTS("TestParentheses",4, -1.5 == res);
	PRINTTESTRESULTS("TestParentheses_calc",5, 0 == Calc(sentence3, &res));
	PRINTTESTRESULTS("TestParentheses",6, 16638 == res);
	PRINTTESTRESULTS("TestParentheses_calc",7, 0 == Calc(sentence4, &res));
	PRINTTESTRESULTS("TestParentheses",8, -74811 == res);
	PRINTTESTRESULTS("TestParentheses_calc",9, 0 == Calc(sentence5, &res));
	PRINTTESTRESULTS("TestParentheses",10, -10 == res);
}

void TestPower()
{
	char *sentence1 = "4^2";
	char *sentence2 = "2^5";
	char *sentence3 = "2^3^2";
	char *sentence4 = "6^2^1^0";
	char *sentence5 = "1^2^3^4^5^6";
	char *sentence6 = "5^(-2)";
	char *sentence7 = "(-2)^3";
	char *sentence8 = "(-2)^4";
	
	double res = 0;
		
	PRINTTESTRESULTS("TestPower_calc",1, 0 == Calc(sentence1, &res));
	PRINTTESTRESULTS("TestPower",2, 16 == res);
	PRINTTESTRESULTS("TestPower_calc",3, 0 == Calc(sentence2, &res));
	PRINTTESTRESULTS("TestPower",4, 32 == res);
	PRINTTESTRESULTS("TestPower_calc",5, 0 == Calc(sentence3, &res));
	PRINTTESTRESULTS("TestPower",6, 512 == res);
	PRINTTESTRESULTS("TestPower_calc",7, 0 == Calc(sentence4, &res));
	PRINTTESTRESULTS("TestPower",8, 36 == res);
	PRINTTESTRESULTS("TestPower_calc",9, 0 == Calc(sentence5, &res));
	PRINTTESTRESULTS("TestPower",10, 1 == res);
	PRINTTESTRESULTS("TestPower_calc",11, 0 == Calc(sentence6, &res));
	PRINTTESTRESULTS("TestPower",12, 0.04 == res);
	PRINTTESTRESULTS("TestPower_calc",13, 0 == Calc(sentence7, &res));
	PRINTTESTRESULTS("TestPower",14, -8 == res);
	PRINTTESTRESULTS("TestPower_calc",15, 0 == Calc(sentence8, &res));
	PRINTTESTRESULTS("TestPower",16, 16 == res);
}

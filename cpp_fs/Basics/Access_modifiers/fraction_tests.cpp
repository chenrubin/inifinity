#include <stdio.h>

#include "fraction.hpp"
#include "MyUtils.h"

int main()
{
	Fraction frac1;
	Fraction frac2(2);
	Fraction frac3(3,2);
	
	printf("Expected result \n0/1\n2/1\n3/2\n\n");
	printf("Actual result\n");
	frac1.Print();
	frac2.Print();
	frac3.Print();
	printf("\n");
	
	PRINTTESTRESULTS("IsEqual",1, 1 == frac1.IsEqual(0, 1));
	frac1.AddToSelf(5, -17);
	PRINTTESTRESULTS("IsEqual",2, 1 == frac1.IsEqual(5, -17));
	PRINTTESTRESULTS("IsEqual",3, 1 == frac1.IsEqual(-10, 34));
	printf("Expected result \n5/-17\n\n");
	printf("Actual result\n");
	frac1.Print();
	printf("\n");
	
	PRINTTESTRESULTS("IsEqual",4, 0 == frac1.IsEqual(5, -18));
	PRINTTESTRESULTS("GetNumer",5, 5 == frac1.GetNumer());
	PRINTTESTRESULTS("GetNumer",6, -17 == frac1.GetDenomin());
	frac2.SetNumer(-6);
	PRINTTESTRESULTS("IsEqual",7, 1 == frac2.IsEqual(-6));
	PRINTTESTRESULTS("IsEqual",8, 0 == frac2.IsEqual(-2));
	frac2.SetDenomin(3);
	PRINTTESTRESULTS("IsEqual",9, 1 == frac2.IsEqual(-2));
	printf("Expected result \n5/-17\n-6/3\n3/2\n\n");
	printf("Actual result\n");
	frac1.Print();
	frac2.Print();
	frac3.Print();
	
	printf("\n");
	frac3.AddToSelf(2,3);
	PRINTTESTRESULTS("IsEqual",10, 1 == frac3.IsEqual(13,6));
	
	printf("Expected result \n5/-17\n-6/3\n13/6\n\n");
	printf("Actual result\n");
	frac1.Print();
	frac2.Print();
	frac3.Print();
	printf("\n");
	
	return 0;
}

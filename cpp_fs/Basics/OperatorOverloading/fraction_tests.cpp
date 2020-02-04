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
	
	/**/
	Fraction frac4;
	Fraction frac5(2);
	Fraction frac6(3,2);
	Fraction frac7(3,2);
	printf("Expected result \n0/1\n2/1\n3/2\n\n");
	printf("Actual result\n");
	frac4.Print();
	frac5.Print();
	frac6.Print();
	printf("\n");
	
	PRINTTESTRESULTS("operator==",1, 1 == (frac6 == frac7));
	PRINTTESTRESULTS("operator==",2, 0 == (frac6 == frac5));
	frac4 = frac6 + frac7;
	PRINTTESTRESULTS("GetNumer",3, 12 == (frac4.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",4, 4 == (frac4.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",5, 3 == (frac6.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",6, 2 == (frac6.GetDenomin()));
	frac4 = frac6 - frac5;
	PRINTTESTRESULTS("GetNumer",7, -1 == (frac4.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",8, 2 == (frac4.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",9, 3 == (frac6.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",10, 2 == (frac6.GetDenomin()));
	
	frac4 = frac6 + 2;
	PRINTTESTRESULTS("GetNumer",11, 7 == (frac4.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",12, 2 == (frac4.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",13, 3 == (frac6.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",14, 2 == (frac6.GetDenomin()));
	
	frac4 = frac6 - 2;
	PRINTTESTRESULTS("GetNumer",15, -1 == (frac4.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",16, 2 == (frac4.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",17, 3 == (frac6.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",18, 2 == (frac6.GetDenomin()));
	
	frac5 = frac6++;
	PRINTTESTRESULTS("GetNumer",23, 3 == (frac5.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",24, 2 == (frac5.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",25, 5 == (frac6.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",26, 2 == (frac6.GetDenomin()));
	
	frac5 = frac6--;
	PRINTTESTRESULTS("GetNumer",27, 5 == (frac5.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",28, 2 == (frac5.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",29, 3 == (frac6.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",30, 2 == (frac6.GetDenomin()));
	
	frac5 = ++frac6;
	PRINTTESTRESULTS("GetNumer",23, 5 == (frac5.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",24, 2 == (frac5.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",25, 5 == (frac6.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",26, 2 == (frac6.GetDenomin()));
	
	frac5 = --frac6;
	PRINTTESTRESULTS("GetNumer",27, 3 == (frac5.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",28, 2 == (frac5.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",29, 3 == (frac6.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",30, 2 == (frac6.GetDenomin()));
	
	Fraction f1(1,2);
	Fraction f2(3,4);
	Fraction f3 = +f1 + -(++f2);
	PRINTTESTRESULTS("GetNumer",31, 1 == (f1.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",32, 2 == (f1.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",33, 7 == (f2.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",34, 4 == (f2.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",35, -10 == (f3.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",36, 8 == (f3.GetDenomin()));
	
	Fraction f4(1,2);
	Fraction f5(3,4);
	f3 = (f4++) - +(f5--);
	PRINTTESTRESULTS("GetNumer",37, 3 == (f4.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",38, 2 == (f4.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",39, -1 == (f5.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",40, 4 == (f5.GetDenomin()));
	PRINTTESTRESULTS("GetNumer",41, -2 == (f3.GetNumer()));
	PRINTTESTRESULTS("GetDenomin",42, 8 == (f3.GetDenomin()));	
		
	return 0;
}

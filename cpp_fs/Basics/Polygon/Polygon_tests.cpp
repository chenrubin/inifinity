#include <stdio.h>

#include "Polygon.h"
#include "point.h"
#include "MyUtils.h"

int main()
{
	int i = 0;
	int point_x5[] = {1,2,3,4,5};
	int point_y5[] = {2,3,4,5,6};
	int point_x7[] = {1,2,3,4,5,6,7};
	int point_y7[] = {2,3,4,5,6,7,8};
	
	Polygon pol1(5);
	Polygon pol2(5);
//	Polygon pol3(5);
	Polygon pol4(7);
	Polygon pol5(7);
	Polygon pol6(7);
	
	for (i = 0; i < 5; ++i)
	{
		Point p(point_x5[i], point_y5[i]);
		pol1.Add(p);
		pol2.Add(p);
		PRINTTESTRESULTS("IsEqual",i, 1 == pol1.IsEqual(pol2));
	}
	
	for (i = 0; i < 7; ++i)
	{
		Point p(point_x7[i], point_y7[i]);
		pol4.Add(p);
		PRINTTESTRESULTS("IsEqual",i, 0 == pol4.IsEqual(pol5));
	}
	for (i = 0; i < 7; ++i)
	{
		Point p(point_x7[i], point_y7[i]);
		pol5.Add(p);
		if (6 == i)
		{
			PRINTTESTRESULTS("IsEqual",i, 1 == pol5.IsEqual(pol4));
		}
		else
		{
			PRINTTESTRESULTS("IsEqual",i, 0 == pol4.IsEqual(pol5));
		}
	}
	
//	PRINTTESTRESULTS("IsEqual",i, 0 == pol3.IsEqual(pol5));
	Polygon pol3(pol5);
	PRINTTESTRESULTS("IsEqual",i, 1 == pol3.IsEqual(pol5));
	
	for (i = 0; i < 7; ++i)
	{
		Point p(point_y7[i], point_x7[i]);
		pol6.Add(p);
	}
	
	PRINTTESTRESULTS("IsEqual",i, 0 == pol6.IsEqual(pol5));
	pol6 = pol5;
	PRINTTESTRESULTS("IsEqual",i, 1 == pol6.IsEqual(pol5));
	
	pol1.Print();
	pol2.Print();
	pol3.Print();
	pol4.Print();
	pol5.Print();
	pol6.Print();
	
	return 0;
}

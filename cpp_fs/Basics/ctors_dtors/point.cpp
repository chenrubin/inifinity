#include <iostream>
#include <stdio.h>
#include <math.h>

#include "point.h"

using namespace std;

double g_Total_Length = 0;

void Point::AdjustBy(Point& p)
{
	x += p.x;
	y += p.y;
	IsCalc = 0;
}

void Point::AdjustBy(int dx, int dy)
{
	x += dx;
	y += dy;
	IsCalc = 0;
}

double Point::Length()
{
	if (0 == IsCalc)
	{
		last_length = sqrt(pow(y, 2) + pow(x, 2));
		IsCalc = 1;
		printf("calculated length\n");
	}
	
	return last_length;
}

void Point::Print(char par1, char par2)
{
	cout << "point is " << par1 << x << "," << y << par2 <<"\n";
}

void Point::Print(Point::paran_type paran)
{
	switch (paran)
	{
		case ROUND:
			printf("point is (%d,%d)\n",x,y);
			break;
		case CURLY:
			printf("point is {%d,%d}\n",x,y);
			break;
		case SQUARE:
			printf("point is [%d,%d]\n",x,y);
			break;
		default:
			printf("point is <%d,%d>\n",x,y);
			break;			
	}	
}

bool Point::IsEqual(Point& p)
{
	return ((x == p.x) && (y == p.y));
}

Point Add(Point p1, Point p2)
{
	Point p_res(p1.x + p2.x, p1.y + p2.y);
	
	return p_res;
}

Point::Point(int a_, int b_) // implementation of a "regular" (non-default) Ctor
	: x(a_), y(b_), IsCalc(0)
{
	g_Total_Length += Length();
	printf("Assign to point\n");
}

Point::~Point()
{
	printf("destruct length\n");
}

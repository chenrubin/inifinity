#include <iostream>
#include <stdio.h>
#include <math.h>

#include "point.h"

using namespace std;

void Point::AdjustBy(Point& p)
{
	x += p.x;
	y += p.y;
}

void Point::AdjustBy(int dx, int dy)
{
	x += dx;
	y += dy;
}

double Point::Length()
{
	return (sqrt(pow(y, 2) + pow(x, 2)));
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
			printf("point is (%f,%f)\n",x,y);
			break;
		case CURLY:
			printf("point is {%f,%f}\n",x,y);
			break;
		case SQUARE:
			printf("point is [%f,%f]\n",x,y);
			break;
		default:
			printf("point is <%f,%f>\n",x,y);
			break;			
	}	
}

Point::Point()
	: x(0), y(0)
{
	printf("Point default Ctor\n");
}

Point::Point(const int a, const int b)
{
	x = a;
	y = b;
}

Point Add(Point p1, Point p2)
{
	Point p_res = {0};
	
	p_res.x = p1.x + p2.x;
	p_res.y = p1.y + p2.y;
	
	return p_res;
}

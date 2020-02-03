#include <iostream>
#include <stdio.h>
#include <math.h>

#include "point.h"

using namespace std;

Point::Point()
	: x(0)
	, y(0)
{}

Point::Point(const int a, const int b)
{
	x = a;
	y = b;
}

Point& Point::operator=(const Point& other_)
{
	x = other_.x;
	y = other_.y;
	
	return *this;
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
			printf("point is (%d,%d)",x,y);
			break;
		case CURLY:
			printf("point is {%d,%d}",x,y);
			break;
		case SQUARE:
			printf("point is [%d,%d]",x,y);
			break;
		default:
			printf("point is <%d,%d>",x,y);
			break;			
	}	
}

int Point::IsEqual(const Point p)
{
	return (x == p.x &&
			y == p.y);
}

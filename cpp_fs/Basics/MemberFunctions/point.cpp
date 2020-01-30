#include <iostream>
#include <math.h>

#include "point.h"

using namespace std;

void Point::AdjustBy(Point p)
{
	x += p.x;
	y += p.y;
}

double Point::Length()
{
	return (sqrt(pow(y, 2) + pow(x, 2)));
}

void Point::Print(char par1, char par2)
{
	cout << "point is " << par1 << x << "," << y << par2 <<"\n";
}

Point Add(Point p1, Point p2)
{
	Point p_res = {0};
	
	p_res.x = p1.x + p2.x;
	p_res.y = p1.y + p2.y;
	
	return p_res;
}

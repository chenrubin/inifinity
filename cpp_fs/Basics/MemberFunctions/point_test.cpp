using namespace std;
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include "point.h"


int main(int argc, char *argv[])
{
	double x = atof(argv[1]) - 1;
	double y = atof(argv[1]) / 2;
	
	Point p1 = {x,y};
	Point p2 = {x,y};
	Point p3 = {x,y};
	Point p4 = {x,y};
	
	cout << "!!!	Print each point	!!!\n";
	cout <<"p1 = ";
	p1.Print('(', ')');
	cout <<"p2 = ";
	p2.Print('[', ']');
	cout <<"p3 = ";
	p3.Print('{', '}');
	
	cout << "!!!	length of each point	!!!\n";	
	cout << "p1 length = "<< p1.Length() <<"\n";
	cout << "p2 length = "<< p2.Length() <<"\n";
	cout << "p3 length = "<< p3.Length() <<"\n";
	
	cout << "!!!	Adjust points	!!!\n";
	cout << "p2 added to p1\n";
	p1.AdjustBy(p2);
	cout <<"p1 = ";
	p1.Print('(', ')');
	
	cout << "p3 added to p2\n";
	p2.AdjustBy(p3);
	cout <<"p2 = ";
	p2.Print('(', ')');
	
	cout << "p1 added to p3\n";
	p3.AdjustBy(p1);
	cout <<"p3 = ";
	p3.Print('(', ')');
		
	p4 = Add(p1, p2);
	cout <<"p4 = ";
	p4.Print('(', ')');
	
	p4 = Add(p2, p3);
	cout <<"p4 = ";
	p4.Print('(', ')');
	
	p4 = Add(p1, p3);
	cout <<"p4 = ";
	p4.Print('(', ')');
	
	return 0;
}

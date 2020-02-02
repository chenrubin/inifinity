using namespace std;
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include "point.h"


int main(int argc, char *argv[])
{	
	Point p1 = {3,4};
	Point p2 = {7,-9};
	Point p3 = {5,1};
	Point p4 = {0,2};
	
	cout << "!!!	Print each point	!!!\n";
	cout <<"p1 = ";
	p1.Print('(', ')');
	cout <<"p2 = ";
	p2.Print('[', ']');
	cout <<"p3 = ";
	p3.Print('{', '}');
	cout <<"p2 = ";
	p2.Print();
	
	cout << "!!!	length of each point	!!!\n";	
	cout << "p1 length = "<< p1.Length() <<"\n";
	cout << "p1 length = "<< p1.Length() <<"\n";
	cout << "p1 length = "<< p1.Length() <<"\n";
	cout << "p2 length = "<< p2.Length() <<"\n";
	cout << "p3 length = "<< p3.Length() <<"\n";
	
	cout << "!!!	Adjust points	!!!\n";
	cout << "p2 added to p1\n";
	p1.AdjustBy(p2);
	cout << "p1 length = "<< p1.Length() <<"\n";
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
	p4.Print('{', ')');
	
	p4 = Add(p2, p3);
	cout <<"p4 = ";
	p4.Print('(', ')');
	
	p4 = Add(p1, p3);
	cout <<"p4 = ";
	p4.Print();
	
	printf("Added by Function overloading excersize\n");
	
	p1.Print(p1.ROUND);
	p1.AdjustBy(3, 4);
	p1.Print(p1.CURLY);
	
	return 0;
}

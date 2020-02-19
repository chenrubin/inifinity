#include <cmath> /* sqrt */
#include <iostream> /* cout */

#include "shapes.hpp"
using namespace ilrd;

void TestLine();
void TestCircle();
void TestRectangle();
void TestSquare();
void TestGroup();

int main()
{
	TestLine();
	TestCircle();
	TestRectangle();
	TestSquare();
	TestGroup();

	return 0;
}

void TestLine()
{
	Point p1(1,1);
	Point p2(0,0);
	Point p3(0.5,0.5);
	double angle = 45;
	double length = sqrt(2);

	Line l1(p1, p2, Shape::WHITE);
	Line l2(p3, angle, Shape::WHITE, length);

	l1.Draw();
	l2.Draw();
	
	Point pos1(1,1);
	Point pos2(0,0);
	l1.SetPosition(pos1);
	l2.SetPosition(pos2);

	l1.Draw();
	l2.Draw();
	std::cout << std::endl;
}

void TestCircle()
{
	Point p1(1,1);
	double radius = 2;

	Circle c1(p1, Shape::WHITE, radius);

	c1.Draw();
	std::cout << std::endl;
}

void TestRectangle()
{
	double height = 2;
	double width = 3;
	Point center(0,0);
	double angle = 0;

	Rectangle rec(center, angle, Shape::BLUE, width, height);

	rec.Draw();
	std::cout << std::endl;
}

void TestSquare()
{
	double edge = 2;
	Point center(0,0);
	double angle = 0;

	Square s(center, angle, Shape::BLUE, edge);

	s.Draw();
	std::cout << std::endl;
}

void TestGroup()
{
	double square_edge = 2;
	Point square_center(0,0);
	double square_angle = 0;

	double rectangle_height = 2;
	double rectangle_width = 3;
	Point rectangle_center(0,0);
	double rectangle_angle = 0;

	Point line1_p1(1,1);
	Point line1_p2(0,0);
	Point line2_p3(2,2);
	double line2_len = 10;
	double line2_angle = 0;

	double circle_radius = 10;
	Point circle_center(5,5);

	Circle c(circle_center, Shape::WHITE, circle_radius);
	Line l1(line1_p1, line1_p2, Shape::WHITE);
	Line l2(line2_p3, line2_angle, Shape::BLUE, line2_len);
	Square sqr(square_center, square_angle, Shape::BLUE, square_edge);
	Rectangle rect(rectangle_center, rectangle_angle, Shape::WHITE, rectangle_width
				   ,rectangle_height);
	GroupOfShapes grp(line2_p3,0,Shape::WHITE);
	grp.AddShape(c);
	grp.Draw();
	grp.AddShape(l1);
	grp.Draw();
	grp.AddShape(l2);
	grp.Draw();
	grp.AddShape(sqr);
	grp.Draw();
	grp.AddShape(rect);
	grp.Draw();

	grp.RemoveShape(l2);
	grp.Draw();
	grp.RemoveShape(sqr);
	grp.Draw();

	grp.Revolve(90);
	grp.Draw();

	std::cout << std::endl;
}
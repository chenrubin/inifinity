#include <iostream> /* cout */
#include <cmath> /* atan */
#include <algorithm> /* find */

#include "shapes.hpp"

#define _USE_MATH_DEFINES
const double degToRad = M_PI / 180;
const double radToDeg = 180 / M_PI;

namespace ilrd
{
static Point CalcMidPointIMP(Point& a_, Point& b_);
//static Point &MaxPointIMP(const Point p1, const Point p2);
static double CalcAngleIMP(Point& a_, Point& b_);
static double CalcLengthIMP(Point& a_, Point& b_);
void CalcPointsIMP(Point *arr, double angle, double len, Point &position);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Point::Point(double x_, double y_)
	: m_x(x_)
	, m_y(y_)
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Point::Point(const Point& other_)
	: m_x(other_.m_x)
	, m_y(other_.m_y)
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Point& Point::operator=(const Point& other_)
{
	m_x = other_.m_x;
	m_y = other_.m_y;

	return *this;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool Point::operator==(const Point& other_)
{
	return (other_.GetX() == GetX() && 
			other_.GetY() == GetY());
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Point::~Point()
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Point::SetX(double x_)
{
	m_x = x_;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Point::SetY(double y_)	// or maybe: void Move(double x_, double y_) ?
{
	m_y = y_;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
double Point::GetX() const
{
	return m_x;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
double Point::GetY() const
{
	return m_y;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Shape::Shape(const Point& position_, double angle_, Color color_)
	: m_position(position_)
	, m_angle(angle_)
	, m_color(color_)
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Shape::Shape(const Shape& other_)
	: m_position(other_.m_position)
	, m_angle(other_.m_angle)
	, m_color(other_.m_color)
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Shape& Shape::operator=(const Shape& other_)
{
	m_position = other_.m_position;
	m_angle = other_.m_angle;
	m_color = other_.m_color;

	return *this;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Shape::SetPosition(const Point& position_)
{
	m_position = position_;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
const Point& Shape::GetPosition() const
{
	return m_position;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Shape::Revolve(double angle_)
{
	Shape::SetAngle(GetAngle() + angle_);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Shape::SetAngle(double angle_)
{
	m_angle = angle_;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
double Shape::GetAngle() const
{
	return m_angle;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Shape::SetColor(Color color_)
{
	m_color = color_;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Shape::Color Shape::GetColor() const
{
	return m_color;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Shape::~Shape()
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Circle::Circle(Point& center_, Color color_, double radius_)
	: Shape(center_, 0, color_)
	, m_radius(radius_)
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Circle::Circle(const Circle& other_)
	: Shape(other_)
	, m_radius(other_.m_radius)
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Circle& Circle::operator=(const Circle& other_)
{
	Shape::operator=(other_);
	m_radius = other_.m_radius;

	return *this;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Circle::Draw() const
{
	std::cout << "!!! Circle !!!" << std::endl;	
	std::cout << "Point = " << GetPosition().GetX() << "," << 
							   GetPosition().GetY() << std::endl 
							  << "radius = " << m_radius << std::endl
							  << "color = " << GetColor() << std::endl
							  << "angle = " << GetAngle() << std::endl; 
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Circle::~Circle()
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Line::Line(Point& midPoint_, double angle_, Color color_, double len_)
	: Shape(midPoint_, angle_, color_)
	, m_len(len_)
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Line::Line(Point& a_, Point& b_, Color color_)
	: Shape(CalcMidPointIMP(a_, b_), CalcAngleIMP(a_, b_) * radToDeg, color_)
	, m_len(CalcLengthIMP(a_, b_))
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Line::~Line()
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static double CalcLengthIMP(Point& a_, Point& b_)
{
	return (sqrt(pow(a_.GetY() - b_.GetY(), 2) + 
				 pow(a_.GetX() - b_.GetX(), 2)));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static double CalcAngleIMP(Point& a_, Point& b_)
{
	if (a_.GetX() == b_.GetX())
	{
		return 0;
	}

	return (atan(a_.GetY() - b_.GetY() / a_.GetX() - b_.GetX()));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static Point CalcMidPointIMP(Point& a_, Point& b_)
{
	Point p_res;

	p_res.SetX((a_.GetX() + b_.GetX()) / 2);
	p_res.SetY((a_.GetY() + b_.GetY()) / 2);

	return p_res;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void CalcPointsIMP(Point *arr, double angle, double len, Point &position)
{
	arr[0].SetX(position.GetX() - cos(angle * degToRad) * (len / 2));
	arr[0].SetY(position.GetY() - sin(angle * degToRad) * (len / 2));
	arr[1].SetX(position.GetX() + cos(angle * degToRad) * (len / 2));
	arr[1].SetY(position.GetY() + sin(angle * degToRad) * (len / 2));
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Line::Draw() const
{
	std::cout << "!!! Line !!!" << std::endl;
	
	Point arr[2];
	Point position = GetPosition();

	CalcPointsIMP(arr, GetAngle(), m_len, position);
	std::cout << "P1 = (" << arr[0].GetX() << "," << arr[0].GetY() << ")" << std::endl;
	std::cout << "P2 = (" << arr[1].GetX() << "," << arr[1].GetY() << ")" << std::endl;
	std::cout << "color = "<< GetColor() << std::endl;
	std::cout << "angle = "<< GetAngle() << std::endl;
	std::cout << "center = " << GetPosition().GetX() << "," << GetPosition().GetY() << std::endl;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Rectangle::Rectangle(Point& center_, double angle_, Color color_,
					 double width_, double height_)
					 : Shape(center_, angle_, color_)
					 , m_width(width_)
					 , m_height(height_)
{}					 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Rectangle::Rectangle(const Rectangle& other_)
	: Shape(other_)
	, m_width(other_.m_width)
	, m_height(other_.m_height)
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Rectangle& Rectangle::operator=(const Rectangle& other_)
{
	Shape::operator=(other_);
	m_height = other_.m_height;
	m_width = other_.m_width;

	return *this;
} 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Rectangle::~Rectangle()
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Rectangle::Draw() const
{
	std::cout << "!!! Rectangle !!!" << std::endl;

	Point arr[2];
	Point pos_arr[2];
	Point position = GetPosition();
	// calculate center of two edges
	CalcPointsIMP(pos_arr, GetAngle(), m_width, position);
	// calculate two points
	CalcPointsIMP(arr, 90 + GetAngle(), m_height, pos_arr[0]);
	std::cout << "P1 = (" << arr[0].GetX() << "," << arr[0].GetY() << ")" << std::endl;
	std::cout << "P2 = (" << arr[1].GetX() << "," << arr[1].GetY() << ")" << std::endl;
	// calculate two other points
	CalcPointsIMP(arr, 90 + GetAngle(), m_height, pos_arr[1]);
	std::cout << "P3 = (" << arr[0].GetX() << "," << arr[0].GetY() << ")" << std::endl;
	std::cout << "P4 = (" << arr[1].GetX() << "," << arr[1].GetY() << ")" << std::endl;

	std::cout << "color = " << GetColor() << std::endl << 
				 "angle = " << GetAngle() << std::endl;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Square::Square(Point& center_, double angle_, Color color_, double edge_)
	: Shape(center_, angle_, color_)
	, m_edge(edge_)
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Square::Square(const Square& other_)
	: Shape(other_)
	, m_edge(other_.m_edge)
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Square& Square::operator=(const Square& other_)
{
	Shape::operator=(other_);
	m_edge = other_.m_edge;

	return *this;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Square::~Square()
{}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void Square::Draw() const
{
	std::cout << "!!! Square !!!" << std::endl;

	Point arr[2];
	Point pos_arr[2];
	Point position = GetPosition();
	// calculate center of two edges
	CalcPointsIMP(pos_arr, GetAngle(), m_edge, position);
	// calculate two points
	CalcPointsIMP(arr, 90 + GetAngle(), m_edge, pos_arr[0]);
	std::cout << "P1 = (" << arr[0].GetX() << "," << arr[0].GetY() << ")" << std::endl;
	std::cout << "P2 = (" << arr[1].GetX() << "," << arr[1].GetY() << ")" << std::endl;
	// calculate two other points
	CalcPointsIMP(arr, 90 + GetAngle(), m_edge, pos_arr[1]);
	std::cout << "P3 = (" << arr[0].GetX() << "," << arr[0].GetY() << ")" << std::endl;
	std::cout << "P4 = (" << arr[1].GetX() << "," << arr[1].GetY() << ")" << std::endl;
	std::cout << "color = " << GetColor() << std::endl << 
				 "angle = " << GetAngle() << std::endl;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
GroupOfShapes::GroupOfShapes(Point& center_, double angle_, Color color_)
	: Shape(center_, angle_, color_)
{}

GroupOfShapes::~GroupOfShapes()
{}

void GroupOfShapes::AddShape(Shape& shape_)
{
	shape_.SetColor(GetColor());
	shape_.SetAngle(GetAngle());
	shape_.SetPosition(GetPosition());
	m_shapes.push_back(&shape_);
}

void GroupOfShapes::RemoveShape(const Shape& shape_)
{
	std::vector<Shape *>::iterator it;
	it = std::find(m_shapes.begin(), m_shapes.end(), &shape_);

	if (it != m_shapes.end())
	{
		m_shapes.erase(it);
	}
}

void GroupOfShapes::SetPosition(const Point& position_)
{
	std::vector<Shape *>::iterator it;

	for (size_t i = 0; i < m_shapes.size(); ++i)
	{
		m_shapes[i]->SetPosition(position_);
	}
}

void GroupOfShapes::SetAngle(double angle_)
{
	std::vector<Shape *>::iterator it;

	for (size_t i = 0; i < m_shapes.size(); ++i)
	{
		m_shapes[i]->SetAngle(angle_);
	}
}

void GroupOfShapes::SetColor(Color color_)
{
	std::vector<Shape *>::iterator it;

	for (size_t i = 0; i < m_shapes.size(); ++i)
	{
		m_shapes[i]->SetColor(color_);
	}
}

void GroupOfShapes::Draw() const
{
	std::cout << "!!!!!!!!!!!!!!!! Group !!!!!!!!!!!!!!" << std::endl;

	for (size_t i = 0; i < m_shapes.size(); ++i)
	{
		m_shapes[i]->Draw();
	}
}

void GroupOfShapes::Revolve(double angle)
{
	for (size_t i = 0; i < m_shapes.size(); ++i)
	{
		m_shapes[i]->Revolve(angle);
	}
}

} // end of namespace ilrdgrp.RemoveShape(l2);
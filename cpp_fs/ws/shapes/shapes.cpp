#include <iostream>

#include "shapes.hpp"
namespace ilrd
{
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
Shape::Shape(Point& position_, double angle_, Color color_)
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

Circle::Circle(const Circle& other_)
	: Shape(other_)
	, m_radius(other_.m_radius)
{}

Circle& Circle::operator=(const Circle& other_)
{
	Shape::operator=(other_);
	m_radius = m_radius;

	return *this;
}

void Circle::Draw() const
{
	std::cout << "Point = " << this->GetPosition().GetX() << "," << this->GetPosition().GetY() << "and radius = " << m_radius << std::endl; 
}

void Circle::Revolve()
{
	std::cout << "Do Nothing" << std::endl; 
}

Circle::~Circle()
{}

/*
explicit Line(Point& a_, Point& b_, Color color_);
	explicit Line(Point& midPoint_, double angle_, Color color_,
			double len_); // both ctors ?  
	Line(const Line& other_); 
	Line& operator=(const Line& other_);
	~Line();
	void Draw() const;
	void Revolve();
Line::Line(Point& a_, Point& b_, Color color_)
{
	m_
}*/
} // end of namespace ilrd
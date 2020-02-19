// shapes.hpp
// Last update: 18/02/2020
// Author: RD 78-79
/*----------------------------------------------------------------------------*/
#ifndef __SHAPES_HPP__
#define __SHAPES_HPP__ 
/*----------------------------------------------------------------------------*/
#include <vector>	// std::vector
/*----------------------------------------------------------------------------*/
namespace ilrd
{
/*----------------------------------------------------------------------------*/
class Point	// should we use the point from the email? or our own point class?
{
public:
	explicit Point(double x_ = 0, double y_ = 0);	// default args = 0?
	Point(const Point& other_);
	Point& operator=(const Point& other_);
	bool operator==(const Point& other_);
	~Point();

	void SetX(double x_);
	void SetY(double y_);	// or maybe: void Move(double x_, double y_) ?
	double GetX() const;
	double GetY() const;

private:
	double m_x;
	double m_y;
};

class Shape
{
public:
	enum Color { WHITE, BLUE /* TODO */}; // enum from the lib / decoupling??
	explicit Shape(const Point& position_, double angle_, Color color_);
	Shape(const Shape& other_);
	Shape& operator=(const Shape& other_);
	virtual ~Shape();

	virtual void SetPosition(const Point& position_);
	const Point& GetPosition() const;
	virtual void SetAngle(double angle_);
	double GetAngle() const;
	virtual void SetColor(Color color_);
	Color GetColor() const;
	virtual void Draw() const = 0;
	virtual void Revolve(double angle_);

private:
	Point m_position;
	double m_angle;
	Color m_color;
};

class Circle : public Shape
{
public:
	explicit Circle(Point& center_, Color color_, double radius_);
	Circle(const Circle& other_);
	Circle& operator=(const Circle& other_);
	~Circle();
	void Draw() const;

private:
	double m_radius;
};

class Line : public Shape
{
public:
	explicit Line(Point& a_, Point& b_, Color color_);
	explicit Line(Point& midPoint_, double angle_, Color color_,
			double len_); // both ctors ?  
	Line(const Line& other_); 
	Line& operator=(const Line& other_);
	~Line();
	void Draw() const;

private:
	double m_len;
};

class Rectangle : public Shape
{
public:
	explicit Rectangle(Point& center_, double angle_, Color color_, 
			double width_, double height_);
	Rectangle(const Rectangle& other_);
	Rectangle& operator=(const Rectangle& other_);
	~Rectangle();
	void Draw() const;

private:
	double m_width;
	double m_height;
};

class Square : public Shape
{
public:
	explicit Square(Point& center_, double angle_, Color color_, double edge_);
	Square(const Square& other_);
	Square& operator=(const Square& other_);
	~Square();
	void Draw() const;

private:
	double m_edge;
};

class GroupOfShapes : public Shape
{
public:
	explicit GroupOfShapes(Point& center_, double angle_, Color color_);
	~GroupOfShapes();
	void AddShape(Shape& shape_);
	void RemoveShape(const Shape& shape_);
	void SetPosition(const Point& position_);
	void SetAngle(double angle_);
	void SetColor(Color color_);
	void Draw() const;
	void Revolve(double angle);

private:
	std::vector<Shape*> m_shapes;
	std::vector<Shape *>::iterator FindShapeInContainerIMP(Shape& shape_);
};

/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __SHAPES_HPP__

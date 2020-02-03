#include <stddef.h> /*size_t*/
#include <stdio.h> /*printf*/

struct Polygon
{
	// numPoints - initial number all of them with value (0,0)
	explicit Polygon(size_t numPoints = 0);
	
	// dtor
	~Polygon();
	
	//CCtor
	Polygon(const &Polygon other_);
	
	// operator
	Polygon& operator=(const &Polygon other_);
	
	void Add(const Point &p);
	bool IsEqual(const Polygon& other) const;
	void Print() const;
	
	size_t m_numPonts;
	Point *m_points;
}

Polygon::Polygon(size_t numPoints)
	:numPoints(0)
{
	numPoints = numPoints;
	m_points = new Point[numPoints];
}

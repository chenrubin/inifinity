#ifndef __ILRD_POLYGON__ 
#define __ILRD_POLYGON__

#include <stddef.h> /*size_t*/
#include <stdio.h> /*printf*/

#include "point.h"

struct Polygon
{
	// numPoints - initial number all of them with value (0,0)
	explicit Polygon(size_t numPoints = 0);
	
	// dtor
	~Polygon();
	
	//CCtor
	Polygon(const Polygon& other_);
	
	// operator
	Polygon& operator=(const Polygon& other_);
	
	void Add(const Point &p);
	bool IsEqual(const Polygon& other_) const;
	void Print() const;
	
	size_t m_numPoints;
	Point *m_points;
};
#endif

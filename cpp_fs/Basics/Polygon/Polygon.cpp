#include <stdio.h> /*printf*/
#include <assert.h> /* assert */

#include "Polygon.h"
#include "point.h"

Polygon::Polygon(size_t numPoints)
	:m_numPoints(0)
{
	m_numPoints = numPoints;
	m_points = new Point[m_numPoints];
	m_numPoints = 0;
}

Polygon::~Polygon()
{
	delete[] m_points; 
}

Polygon::Polygon(const Polygon& other_)
	: m_numPoints(other_.m_numPoints)
{
	size_t i = 0;
	m_points = new Point[m_numPoints];
	
	for (i = 0; i < m_numPoints; ++i)
	{
		m_points[i] = other_.m_points[i];
	}
}

Polygon& Polygon::operator=(const Polygon& other_)
{	
	m_numPoints = other_.m_numPoints;
	size_t i = 0;
	
	for (i = 0; i < m_numPoints; ++i)
	{
		m_points[i] = other_.m_points[i];
	}
	
	return *this;
}
	
void Polygon::Add(const Point &p)
{
	m_points[m_numPoints] = p;
	
	++m_numPoints;
}
	
bool Polygon::IsEqual(const Polygon& other_) const
{
	if (m_numPoints == other_.m_numPoints)
	{
		size_t i = 0;
		for (i = 0; i < m_numPoints; ++i)
		{
			if (!(m_points[i].IsEqual(other_.m_points[i])))
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	
	return true;
}

void Polygon::Print() const
{
	size_t i = 0;
	
	for (i = 0; i < m_numPoints; ++i)
	{
		m_points[i].Print(Point::ROUND);
	}
	printf("\n");
}

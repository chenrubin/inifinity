#ifndef __ILRD_POINT__ 
#define __ILRD_POINT__

struct Point
{
	enum paran_type
	{
		ROUND,
		SQUARE,
		CURLY,
		ANGULAR
	};
	explicit Point();
	explicit Point(const int a, const int b);
	Point& operator=(const Point& other_);
	int IsEqual(const Point p2);
	void Print(char par1, char par2);
	void Print(enum paran_type);
	
	int x;
	int y;
};
#endif

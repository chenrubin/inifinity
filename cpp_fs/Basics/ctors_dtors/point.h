#ifndef __POINT_H__
#define __POINT_H__

extern double g_Total_Length;

struct Point
{
	enum paran_type
	{
		ROUND,
		SQUARE,
		CURLY,
		ANGULAR
	};
	void AdjustBy(Point& p);
	void AdjustBy(int dx, int dy);
	double Length();
	void Print(char par1 = '(', char par2 = '>');
	void Print(enum paran_type);
	bool IsEqual(Point& p);
	explicit Point(); // initialization function without parameter is 
				// called the Default Constructor often
				// abbreviated ass Ctor)
				
	explicit Point(int a_, int b_); // regular (non default) Ctor
	
	~Point(); //de-initialization function is called destructor (Dtor)
	
	int x;
	int y;
	double last_length;
	int IsCalc;	
};

Point Add(Point p1, Point p2);

#endif

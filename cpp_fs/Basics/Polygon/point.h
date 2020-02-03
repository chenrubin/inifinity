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
	void AdjustBy(Point& p);
	void AdjustBy(int dx, int dy);
	double Length();
	void Print(char par1, char par2);
	void Print(enum paran_type);
	
	double x;
	double y;
};

Point Add(Point p1, Point p2);

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
	
	int x;
	int y;
	double last_length;
	int IsCalc = 0;	
};

Point Add(Point p1, Point p2);

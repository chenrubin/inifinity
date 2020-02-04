#ifndef __FRACTION_HPP__
#define __FRACTION_HPP__

class Fraction
{
public:
	explicit Fraction();
	explicit Fraction(int numerator_);
	explicit Fraction(int numerator_, int denominator_);
	bool IsUndefined();
	void AddToSelf(int numerator_, int denominator_);
	void AddToSelf(int a_);
	void Print();
	bool IsEqual(int numerator_, int denominator_);
	bool IsEqual(int numerator_);	
	int GetNumer();
	int GetDenomin();
	void SetNumer(int val);
	void SetDenomin(int val);
	
private:	
	int m_numerator;
	int m_denominator;
};
#endif

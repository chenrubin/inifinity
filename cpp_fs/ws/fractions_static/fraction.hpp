#ifndef __FRACTION_HPP__
#define __FRACTION_HPP__

class Fraction
{
private:
	static size_t frac_counter; 
	
public:
	explicit Fraction();
	explicit Fraction(int numerator_);
	explicit Fraction(int numerator_, int denominator_);
	Fraction(const Fraction& other);
	~Fraction();
	bool IsUndefined() const;
	void AddToSelf(int numerator_, int denominator_);
	void AddToSelf(int a_);
	void Print();
	bool IsEqual(int numerator_, int denominator_);
	bool IsEqual(int numerator_);	
	int GetNumer() const;
	int GetDenomin() const;
	void SetNumer(int val);
	void SetDenomin(int val);
	
	Fraction operator+(const Fraction& frac1);
	Fraction operator-(const Fraction& frac1);
	Fraction operator+(int num);
	Fraction operator-(int num);
	Fraction& operator++();
	Fraction& operator--();
	Fraction operator++(int);
	Fraction operator--(int);
	Fraction operator+();
	Fraction operator-();
	Fraction& operator+=(const Fraction& other_);
	Fraction& operator-=(const Fraction& other_);
	Fraction& operator=(const Fraction& other_);
	bool operator==(const Fraction& other_) const;
	
	static int getNumOfFractions();
	
private:	
	int m_numerator;
	int m_denominator;
};

#endif

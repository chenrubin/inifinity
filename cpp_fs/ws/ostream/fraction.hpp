#ifndef __FRACTION_HPP__
#define __FRACTION_HPP__

namespace ilrd
{
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

std::ostream& operator<<(std::ostream& os_, const Fraction& frac);

inline Fraction Fraction::operator+(int num)
{
	int num_numer = num; 
	int num_denonim = 1;
	int new_numer = m_numerator * num_denonim + num_numer * m_denominator;
	int new_denomin = num_denonim * m_denominator;
		
	return Fraction(new_numer, new_denomin);
}

inline Fraction Fraction::operator-(int num)
{
	int num_numer = num; 
	int num_denonim = 1;
	int new_numer = m_numerator * num_denonim - num_numer * m_denominator;
	int new_denomin = num_denonim * m_denominator;
		
	return Fraction(new_numer, new_denomin);
}

} // end of namespace ilrd
#endif

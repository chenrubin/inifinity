#include <cstdio> /* printf */
#include <assert.h> /* assert */
#include <iostream> /* cout */

#include "fraction.hpp"

#define DEFAULT_DENOMINATOR 1

namespace ilrd
{
size_t Fraction::frac_counter = 0; 

static void ExpandFractionIMP(int *numerator, int *denominator, int expand_by);

Fraction::Fraction()
	: m_numerator(0)
	, m_denominator(1)
{
	++frac_counter;
}

Fraction::Fraction(int numerator_)
	: m_numerator(numerator_)
	, m_denominator(DEFAULT_DENOMINATOR)
{
	++frac_counter;
}

Fraction::Fraction(int numerator_, int denominator_)
	: m_numerator(numerator_)
	, m_denominator(denominator_)
{
	++frac_counter;
}

Fraction::Fraction(const Fraction& other)
	: m_numerator(other.m_numerator)
	, m_denominator(other.m_denominator)
{
	++frac_counter;
}



// conversion from A (assignment):
Fraction& Fraction::operator=(const int num)
{
	m_numerator = num;
	m_denominator = 1;
	
	return *this;
}
//Fraction to int
/*Fraction::operator int()
{
	return m_numerator;
}*/
/**/
//Fraction to double
Fraction::operator double()
{
	return (double)m_numerator / (double)m_denominator;
}

bool Fraction::IsUndefined() const
{
	return (0 == m_denominator);
}

void Fraction::AddToSelf(int numerator_, int denominator_)
{
	assert(!IsUndefined());
	
	SetNumer(m_numerator * denominator_ + numerator_ * m_denominator);
	SetDenomin(denominator_ * m_denominator);
}

void Fraction::AddToSelf(int a_)
{
	AddToSelf(a_, 1);
}

void Fraction::Print()
{
	assert(!IsUndefined());
	
	printf("%d/%d\n", m_numerator, m_denominator);
}

bool Fraction::IsEqual(int numerator_, int denominator_)
{
	assert(!IsUndefined());
	assert(0 != denominator_);
	
	int frac_denominator = GetDenomin();
	int frac_numerator = GetNumer();
	int temp_denom = denominator_;
	
	ExpandFractionIMP(&numerator_, &denominator_, frac_denominator);
	ExpandFractionIMP(&frac_numerator, &frac_denominator, temp_denom);
	
	if (numerator_ == frac_numerator &&
		denominator_ == frac_denominator)
	{
		return true;
	}
	
	return false;
}

bool Fraction::IsEqual(int numerator_)
{
	return (IsEqual(numerator_, 1));
}

int Fraction::GetNumer() const
{
	assert(!IsUndefined());
	
	return m_numerator;
}

int Fraction::GetDenomin() const
{
	assert(!IsUndefined());
	
	return m_denominator;
}
	
void Fraction::SetNumer(int val)
{
	assert(!IsUndefined());
	
	m_numerator = val;
}

void Fraction::SetDenomin(int val)
{
	assert(!IsUndefined());
	
	m_denominator = val;
}

Fraction::~Fraction()
{
	--frac_counter;
	printf("counter = %ld\n", frac_counter);
}

static void ExpandFractionIMP(int *numerator, int *denominator, int expand_by)
{
	assert(0 != *denominator);
	
	*numerator *= expand_by;
	*denominator *= expand_by;
}
/*
double operator+(const int num, const Fraction& frac)
{
	int new_denomin = frac.GetDenomin();
	int new_numer = num * frac.GetDenomin() + frac.GetNumer();
	
	return ((double)new_numer / (double)new_denomin);
}

double operator-(const int num, const Fraction& frac)
{
	int new_denomin = frac.GetDenomin();
	int new_numer = num * frac.GetDenomin() - frac.GetNumer();
	
	return ((double)new_numer / (double)new_denomin);
}
*/
std::ostream& operator<<(std::ostream& os_, const Fraction& frac)
{
	return (os_ << frac.GetNumer() << "/" << frac.GetDenomin());	
}

Fraction& Fraction::operator++()
{
	m_numerator += m_denominator; 
	
	return *this;
}

Fraction& Fraction::operator--()
{
	m_numerator -= m_denominator; 
	
	return *this;
}

Fraction Fraction::operator++(int)
{
	Fraction temp_frac = *this;
	++(*this);
	
	return temp_frac;
}
	
Fraction Fraction::operator--(int)
{
	Fraction temp_frac = *this;
	--(*this);
	
	return temp_frac;
}

Fraction Fraction::operator+()
{
	return *this;
}

Fraction Fraction::operator-()
{
	m_numerator = -m_numerator;
	
	return *this;
}

Fraction& Fraction::operator=(const Fraction& other_)
{
	SetNumer(other_.GetNumer());
	SetDenomin(other_.GetDenomin());
	
	return *this;
}

bool Fraction::operator==(const Fraction& other_) const
{
	assert(!IsUndefined());
	assert(0 != other_.GetDenomin());
	
	int frac_denominator = GetDenomin();
	int frac_numerator = GetNumer();
	int temp_denom = other_.GetDenomin();
	int other_denom = other_.GetDenomin();
	int other_numerator = other_.GetNumer();
	
	ExpandFractionIMP(&other_numerator, &other_denom, frac_denominator);
	ExpandFractionIMP(&frac_numerator, &frac_denominator, temp_denom);
	
	if (other_numerator == frac_numerator &&
		other_denom == frac_denominator)
	{
		return true;
	}
	
	return false;
}

Fraction Fraction::operator+(const Fraction& frac1)
{
	int numer = frac1.GetNumer() * GetDenomin() + 
			 	GetNumer() * frac1.GetDenomin();
	int denonim = frac1.GetDenomin() * GetDenomin();
	
	return Fraction(numer, denonim);
}

Fraction Fraction::operator-(const Fraction& frac1)
{
	int numer = GetNumer() * frac1.GetDenomin() - 
				frac1.GetNumer() * GetDenomin();
	int denonim = frac1.GetDenomin() * GetDenomin();
	
	return Fraction(numer, denonim);
}

Fraction& Fraction::operator+=(const Fraction& other_)
{
	assert(!IsUndefined());
	
	SetNumer(m_numerator * other_.GetDenomin() + 
			 other_.GetNumer() * m_denominator);
	SetDenomin(other_.GetDenomin() * m_denominator);
	
	return *this;
}

Fraction& Fraction::operator-=(const Fraction& other_)
{
	assert(!IsUndefined());
	
	SetNumer(m_numerator * other_.GetDenomin() - 
			 other_.GetNumer() * m_denominator);
	SetDenomin(other_.GetDenomin() * m_denominator);
	
	return *this;
}

int Fraction::getNumOfFractions()
{ 
	return frac_counter; 
}
} // end of namespace ilrd

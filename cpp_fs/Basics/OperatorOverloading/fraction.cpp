#include <cstdio> /* printf */
#include <assert.h> /* assert */

#include "fraction.hpp"

static void ExpandFractionIMP(int *numerator, int *denominator, int expand_by);

Fraction::Fraction()
	: m_numerator(0)
	, m_denominator(1)
{}

Fraction::Fraction(int numerator_)
	: m_numerator(numerator_)
	, m_denominator(1)
{}

Fraction::Fraction(int numerator_, int denominator_)
	: m_numerator(numerator_)
	, m_denominator(denominator_)
{}

Fraction Fraction::operator+()
{
	return *this;
}

Fraction Fraction::operator-()
{
	int numerator = m_numerator * (-1);
	Fraction frac(numerator, m_denominator);
	
	return frac;
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

bool Fraction::IsUndefined() const
{
	return (0 == m_denominator);
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

void Fraction::AddToSelf(int numerator_, int denominator_)
{
	assert(!IsUndefined());
	
	SetNumer(m_numerator * denominator_ + numerator_ * m_denominator);
	SetDenomin(denominator_ * m_denominator);
}

Fraction Fraction::operator+(const int num)
{
	int num_numer = num; 
	int num_denonim = 1;
	int new_numer = m_numerator * num_denonim + num_numer * m_denominator;
	int new_denomin = num_denonim * m_denominator;
		
	return Fraction(new_numer, new_denomin);
}

Fraction Fraction::operator-(const int num)
{
	int num_numer = num; 
	int num_denonim = 1;
	int new_numer = m_numerator * num_denonim - num_numer * m_denominator;
	int new_denomin = num_denonim * m_denominator;
		
	return Fraction(new_numer, new_denomin);
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

static void ExpandFractionIMP(int *numerator, int *denominator, int expand_by)
{
	assert(0 != *denominator);
	
	*numerator *= expand_by;
	*denominator *= expand_by;
}

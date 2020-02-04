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

bool Fraction::IsUndefined()
{
	return (0 == m_denominator);
}

void Fraction::AddToSelf(int numerator_, int denominator_)
{
	assert(!IsUndefined());
	
	m_numerator = m_numerator * denominator_ + 
				  numerator_ * m_denominator;
	m_denominator = denominator_ * m_denominator;
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

int Fraction::GetNumer()
{
	assert(!IsUndefined());
	
	return m_numerator;
}

int Fraction::GetDenomin()
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

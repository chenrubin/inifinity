// String - Source code
// Last update: 05/02/2020
// Author: Hana

/*----------------------------------------------------------------------------*/

#include <cstddef>	// size_t 
#include <cassert>	// assert
#include <cstring>	// strlen(), strcmp()

#include "string.hpp"	// ilrd::String

/*----------------------------------------------------------------------------*/

static int StringCmp(const ilrd::String& str1, const ilrd::String& str2);

/*----------------------------------------------------------------------------*/

ilrd::String::String(const char* str)
		: m_str(NULL)
{
	assert(NULL != str);

	m_str = new char[strlen(str) + 1];
	strcpy(m_str, str);
}

/*----------------------------------------------------------------------------*/

ilrd::String::String(const /*ilrd::*/String& other)
	: m_str(NULL)
{
	*this = other;		
}

/*----------------------------------------------------------------------------*/

ilrd::String& ilrd::String::operator=(const /*ilrd::*/String& other)
{
	assert(NULL != other.Cstr());

	delete[] m_str;
	m_str = new char[strlen(other.Cstr()) + 1];
	strcpy(m_str, other.Cstr());

	return *this;
}

/*----------------------------------------------------------------------------*/

ilrd::String::~String()
{
	delete[] m_str;
}

/*----------------------------------------------------------------------------*/

size_t ilrd::String::Length(void) const
{
	assert(NULL != Cstr());

	return strlen(Cstr());
}

/*----------------------------------------------------------------------------*/

const char* ilrd::String::Cstr(void) const
{
	return m_str;
}	

/*----------------------------------------------------------------------------*/

bool ilrd::operator==(const /*ilrd::*/String& str1, const /*ilrd::*/String& str2)
{
	return (0 == StringCmp(str1, str2));
}

bool ilrd::operator>(const ilrd::String& str1, const ilrd::String& str2)
{
	return (0 < StringCmp(str1, str2));
}
    
bool ilrd::operator<(const ilrd::String& str1, const ilrd::String& str2)
{
	return (0 > StringCmp(str1, str2));
}
/*	
bool ilrd::operator>=(const ilrd::String& str1, const ilrd::String& str2)
{
	return (0 <= StringCmp(str1, str2));
}

bool ilrd::operator<=(const ilrd::String& str1, const ilrd::String& str2)
{
	return (0 >= StringCmp(str1, str2));
}
*/
/*----------------------------------------------------------------------------*/

static int StringCmp(const ilrd::String& str1, const ilrd::String& str2)
{
	assert(NULL != str1.Cstr());
	assert(NULL != str2.Cstr());

	return strcmp(str1.Cstr(), str2.Cstr());
}

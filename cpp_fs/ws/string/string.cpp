/****************************
*		Author: Chen		*
*		Reviewer: Ofer		*
*		String				*
*		5/2/2020			*
****************************/

#include <cstring> /* strlen */
#include <ostream> /* namespaces */
#include <cassert> /* assert */
#include <iostream> /* namespaces */

#include "string.hpp"
namespace ilrd
{
String::String(const char *str)
	: m_str(String::StrDupIMP(str))
{}

String::String(const String& other)
	: m_str(String::StrDupIMP(other.Cstr()))
{}

String::~String()
{
	delete[] m_str;
}

String& ilrd::String::operator=(const String& other_)
{
	assert(NULL != other_.m_str);
	
	if (Length() <= other_.Length())
	{
		char *temp_str = StrDupIMP(other_.m_str);
		delete[] m_str;
		m_str = temp_str;
	}
	else
	{
		strcpy(m_str, other_.Cstr());
	}	
	
	return *this;
}

size_t String::Length() const
{
	assert(NULL != m_str);
	
	return strlen(m_str);
}

const char *String::Cstr() const
{
	assert(NULL != m_str);
	
	return m_str;
}

std::ostream& operator<<(std::ostream& os_, const String& str)
{
	return os_ << str.Cstr();
}

bool operator==(const String& str1, const String& str2)
{
	assert(NULL != str1.Cstr());
	assert(NULL != str2.Cstr());
	
	return (0 == strcmp(str1.Cstr(), str2.Cstr()));
}

bool operator>(const String& str1, const String& str2)
{
	assert(NULL != str1.Cstr());
	assert(NULL != str2.Cstr());
	
	return (0 < strcmp(str1.Cstr(), str2.Cstr()));
}

bool operator<(const String& str1, const String& str2)
{
	assert(NULL != str1.Cstr());
	assert(NULL != str2.Cstr());
	
	return (0 > strcmp(str1.Cstr(), str2.Cstr()));
}

char *String::StrDupIMP(const char *str)
{
	assert(str);
	
	char *res_str = new char[strlen(str) + 1];
	strcpy(res_str, str);
		
	return res_str;
}
}// end of namespace ilrd

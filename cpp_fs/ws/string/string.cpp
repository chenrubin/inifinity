/****************************
*		Author: Chen		*
*		Reviewer: Ofer		*
*		String				*
*		5/2/2020			*
****************************/

#include <string.h> /* strlen */
#include <ostream> /* namespaces */
#include <assert.h> /* assert */

#include "string.hpp"

ilrd::String::String(const char *str)
	: m_str(NULL)
{
	assert(NULL != str);
	
	m_str = new char[strlen(str) + 1];
	assert(NULL != m_str);
	strcpy(m_str, str);
}

ilrd::String::String(const String& other)
	: m_str(NULL)
{
	assert(NULL != other.m_str);
	
	m_str = new char[strlen(other.m_str) + 1];
	assert(NULL != m_str);
	strcpy(m_str, other.m_str);
}

ilrd::String::~String()
{
	delete[] m_str;
}

ilrd::String& ilrd::String::operator=(const String& other)
{
	assert(NULL != other.m_str);
	
	if (Length() < other.Length())
	{
		delete[] m_str;
		m_str = new char[other.Length() + 1];
	}
	
	strcpy(m_str, other.m_str);
	
	return *this;
}

size_t ilrd::String::Length() const
{
	assert(NULL != m_str);
	
	return strlen(m_str);
}

const char *ilrd::String::Cstr() const
{
	assert(NULL != m_str);
	
	return m_str;
}	

bool ilrd::operator==(const String& str1, const String& str2)
{
	assert(NULL != str1.Cstr());
	assert(NULL != str2.Cstr());
	
	return (0 == strcmp(str1.Cstr(), str2.Cstr()));
}

bool ilrd::operator>(const String& str1, const String& str2)
{
	assert(NULL != str1.Cstr());
	assert(NULL != str2.Cstr());
	
	return (0 < strcmp(str1.Cstr(), str2.Cstr()));
}

bool ilrd::operator<(const String& str1, const String& str2)
{
	assert(NULL != str1.Cstr());
	assert(NULL != str2.Cstr());
	
	return (0 > strcmp(str1.Cstr(), str2.Cstr()));
}

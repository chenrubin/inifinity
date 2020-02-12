#ifndef __ILRD_STRING_HPP__
#define __ILRD_STRING_HPP__


#include <stddef.h> /* size_t */
#include <iostream> /* size_t */

namespace ilrd
{
class String
{
public:
	String(const char *str = "");
	String(const String& other);
	~String();
	String& operator=(const String& other);
	size_t Length() const;
	const char *Cstr() const;	
	
private:
	static char *StrDupIMP(const char *str);
	char *m_str;	
};

std::ostream& operator<<(std::ostream& os_, const String& str);
bool operator==(const String& str1, const String& str2);
bool operator>(const String& str1, const String& str2);
bool operator<(const String& str1, const String& str2);
}
#endif

// RCString - Header file
// Last update: 24/02/2020
// Author: RD 78-79
/*----------------------------------------------------------------------------*/
#ifndef __RCString_HPP__ 
#define __RCString_HPP__
/*----------------------------------------------------------------------------*/
#include <cstddef>	// size_t
#include <iostream>	// std::ostream&, std::istream&
/*----------------------------------------------------------------------------*/
namespace ilrd
{
class RCString;
   
bool operator==(const RCString& str1_, const RCString& str2_);
bool operator!=(const RCString& str1_, const RCString& str2_);
bool operator>(const RCString& str1_, const RCString& str2_);
bool operator<(const RCString& str1_, const RCString& str2_);
std::ostream& operator<<(std::ostream& os_, const RCString& str_);
std::istream& operator>>(std::istream& is_, RCString& str_);

class RCString
{
public:
	RCString(const char* str_ = "\0");	// non explicit by design
	RCString(const RCString& other_);		// non explicit by design
	RCString& operator=(const RCString& other_);
	~RCString();

	size_t Length() const;
	const char* CStr() const;
    const char& operator[] (size_t index_) const;
    char& operator[] (size_t index_);
    RCString& Concat(const RCString& other_);
	
private:
struct 
	char* m_str;
	size_t* refernce_counter;
};	       
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __RCString_HPP__

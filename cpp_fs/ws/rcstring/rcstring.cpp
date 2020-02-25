#include <iostream>
#include <cstring> /* strcmp, strcpy, strlen */
#include <cassert> /* assert */

#include "rcstring.hpp"
namespace ilrd
{
char *StrDupIMP(const char *str);
size_t *RefInitIMP();

RCString::RCString(const char* str_)
    : m_str(StrDupIMP(str_))
    , refernce_counter(RefInitIMP())
{}

RCString::~RCString()
{
    if (1 == (*refernce_counter))
    {
        delete[] m_str;
        delete refernce_counter;
    }
    else
    {
        m_str = NULL;
        *refernce_counter -= 1;
        refernce_counter = NULL;
    }    
}

RCString::RCString(const RCString& other_)
{
    m_str = other_.m_str;
    refernce_counter = other_.refernce_counter;
    *refernce_counter += 1;
}

RCString& RCString::operator=(const RCString& other_)
{
    if (this != &other_)
    {
        if (1 == *refernce_counter)
        {
            delete []m_str;
            delete refernce_counter;
            m_str = other_.m_str;
            refernce_counter = other_.refernce_counter;
            *refernce_counter += 1;
        }
        else
        {
            m_str = other_.m_str;
            *refernce_counter -= 1;
            refernce_counter = other_.refernce_counter;
            *refernce_counter += 1;
        }
    }

    return *this;
}

size_t RCString::Length() const
{
    return strlen(m_str);
}

const char* RCString::CStr() const
{
    return m_str;
}

const char& RCString::operator[] (size_t index_) const
{
    return (m_str[index_]);
}

char& RCString::operator[] (size_t index_)
{
    if (1 == (*refernce_counter))
    {
        return (m_str[index_]);
    }

    m_str = StrDupIMP(m_str);
    *refernce_counter -= 1;
    refernce_counter = RefInitIMP();

    return m_str[index_];
}

bool operator==(const RCString& str1_, const RCString& str2_)
{
    return (0 == strcmp(str1_.CStr(), str2_.CStr()));
}

bool operator!=(const RCString& str1_, const RCString& str2_)
{
    return (0 != strcmp(str1_.CStr(), str2_.CStr()));
}

bool operator>(const RCString& str1_, const RCString& str2_)
{
    return (0 < strcmp(str1_.CStr(), str2_.CStr()));
}

bool operator<(const RCString& str1_, const RCString& str2_)
{
    return (0 > strcmp(str1_.CStr(), str2_.CStr()));
}

std::ostream& operator<<(std::ostream& os_, const RCString& str_)
{
    os_ << str_.CStr() << std::endl;

    return os_;
}

std::istream& operator>>(std::istream& is_, RCString& str_)
{
    char buf[256];
    is_.getline (buf, 256);
    std::cout << "buf = "<< buf <<"\n";
    str_ = buf;

    return is_;
}

RCString& RCString::Concat(const RCString& other_)
{
    char *new_str = NULL;
    size_t new_len = Length() + other_.Length() + 1;

    try
    {
        new_str = new char[new_len];
    }
    catch(const std::exception& e)
    {
        std::cerr << "Failed to allocate in concatenation: " <<e.what() 
                                                                << std::endl;
    }
    strcpy(new_str, m_str);
    strcat(new_str, other_.CStr());

    if (1 != *refernce_counter)
    {
        *refernce_counter -= 1;
    }
    else
    {
        delete refernce_counter;
        delete[] m_str;
        refernce_counter = RefInitIMP();
    }
    m_str = new_str;

    return *this;
}

char *StrDupIMP(const char *str)
{
    assert(str);

    char *res_str = NULL;
    
    try
    {
      res_str = new char[strlen(str) + 1];
    }
    catch(std::bad_alloc& e)
    {
        std::cerr << "Failed to allocate string " << e.what() << std::endl;
    }
    strcpy(res_str, str);

    return res_str;
}

size_t *RefInitIMP()
{
    size_t *res_ref = NULL;
    
    try
    {
        res_ref = new size_t(1);
    }
    catch(std::bad_alloc& e)
    {
        std::cerr << "Failed to allocate reference counter " << e.what() << std::endl;
    }

    return res_ref;
}

}
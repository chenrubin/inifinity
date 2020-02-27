/*********************************
 * Author: Chen Rubin            *
 * Reviewer: Tsisana             *
 * Date: Feb 26, 2020            *
 * RCString                      *
 * ******************************/

#include <cstring> /* strcmp, strcpy, strlen */
#include <cassert> /* assert */

#include "rcstring.hpp"

#define BUFF_SIZE 256

namespace ilrd
{

RCString::RCString(const char* str_) throw(std::bad_alloc)
    : m_str_mems(PrivateMemsDup(str_))
{}

RCString::~RCString()
{
    UpdateData();
}

RCString::RCString(const RCString& other_)
    : m_str_mems(other_.m_str_mems)
{
    ++(*m_str_mems.refernce_counter);
}

RCString& RCString::operator=(const RCString& other_)
{
    if (this != &other_)
    {
         UpdateData();
         
         m_str_mems.refernce_counter = other_.m_str_mems.refernce_counter;
         m_str_mems.m_str = other_.m_str_mems.m_str;
         ++(*m_str_mems.refernce_counter);    
    }

    return *this;
}

size_t RCString::Length() const
{
    return strlen(m_str_mems.m_str);
}

const char* RCString::CStr() const
{
    return m_str_mems.m_str;
}

const char& RCString::operator[] (size_t index_) const
{
    return (m_str_mems.m_str[index_]);
}

RCString::Proxy RCString::operator[] (size_t index_)
{
    Proxy prox(this, index_);

    return prox;
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
    char buf[BUFF_SIZE];
    is_.getline (buf, BUFF_SIZE);
    std::cout << "buf = "<< buf <<"\n";
    str_ = buf;

    return is_;
}

RCString& RCString::Concat(const RCString& other_) throw(std::bad_alloc)
{
    size_t new_len = Length() + other_.Length() + 1;
    char *buf = new char[sizeof(size_t) + new_len];
    
    strcpy(buf + sizeof(size_t), CStr());
    strcat(buf + sizeof(size_t), other_.CStr());

    UpdateData();

    m_str_mems.m_str = buf + sizeof(size_t);
    m_str_mems.refernce_counter = new (buf) size_t(1);

    return *this;
}

void RCString::UpdateData()
{
    if (1 == *(m_str_mems.refernce_counter))
    {
        delete[] m_str_mems.refernce_counter;
    }
    else
    {
        --(*m_str_mems.refernce_counter);
    }   
}

RCString::Str_priv RCString::PrivateMemsDup(const char *str) throw(std::bad_alloc)
{
    assert(str);

    char *buf;
    buf = new char[sizeof(size_t) + strlen(str) + 1];

    m_str_mems.refernce_counter = new (buf) size_t(1);
    m_str_mems.m_str = new (buf + sizeof(size_t)) char[strlen(str) + 1];
    strcpy(m_str_mems.m_str, str);

    return m_str_mems;
}

RCString::Proxy::Proxy(RCString *rcstr, size_t index)
    : m_rcstr(rcstr)
    , m_index(index)
{}    

RCString::Proxy::~Proxy()
{}

RCString& RCString::Proxy::operator=(const char& ch) throw(std::bad_alloc)
{
    assert(ch);

    if (1 != (*(m_rcstr->m_str_mems.refernce_counter)))
    {
        --(*m_rcstr->m_str_mems.refernce_counter);
        m_rcstr->m_str_mems = m_rcstr->PrivateMemsDup(m_rcstr->m_str_mems.m_str);         
    }

    m_rcstr->m_str_mems.m_str[m_index] = ch;

    return *m_rcstr;
}

RCString::Proxy::operator char() const
{
    return (m_rcstr->m_str_mems.m_str[m_index]);
}
} // end of namespace ilrd
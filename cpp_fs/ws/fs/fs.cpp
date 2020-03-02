#include <iostream> /* cout */
#include <cstring> /* strcmp */
#include <algorithm> /* find */

#include "fs.hpp"

namespace ilrd
{
static std::string IndentIMP(std::string str, size_t level);
static bool CompFuncIMP(Entry *ent1, Entry* ent2);

Entry::Entry(const std::string& name_)
    : m_name(name_)
{}

Entry::~Entry()
{}

const std::string& Entry::GetName() const
{
    return m_name;
}
/**/
File::File(const std::string& name_)
    : Entry(name_)
{}

File::~File()
{}

std::string File::ToString(size_t level) const
{
    return  Entry::GetName();
}
/**/
Directory::Directory(const std::string& name_)
    : Entry(name_)
{}

Directory::~Directory()
{

}

std::string Directory::ToString(size_t level) const
{
    std::string res = Entry::GetName();
    //std::sort(m_entries.begin(), m_entries.end(), CompFuncIMP);
    for (size_t i = 0; i < m_entries.size(); ++i)
    {
        res += '\n';
        res += IndentIMP(m_entries[i]->ToString(level + 1), level + 1);
    }

    return res;
}

void Directory::AddEntry(Entry* entry_)
{
    m_entries.push_back(entry_);
    std::sort(m_entries.begin(), m_entries.end(), CompFuncIMP);
}

Entry* Directory::RemoveEntry(Entry* entry_)
{
    if (GetName() == entry_->GetName())
    {
        return entry_;
    }
    std::vector<Entry *>::iterator it = std::find(m_entries.begin(), 
                                                  m_entries.end(), 
                                                  entry_);
    if (it != m_entries.end())
    {
        m_entries.erase(it);        
    }

    return *it;
}

static std::string IndentIMP(std::string str, size_t level)
{
    std::string res;
    for (size_t i = 0; i < level; ++i)
    {
        res += '\t';
    }

    return res + str;
}

static bool CompFuncIMP(Entry *ent1, Entry* ent2)
{
    return (0 > strcmp(ent1->GetName().c_str(),
                       ent2->GetName().c_str()));
}
  
}
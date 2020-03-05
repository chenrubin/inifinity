#include <iostream> /* cout */
#include <cstring> /* strcmp */
#include <algorithm> /* find */
#include <dirent.h> /* opendir, readdir */

#include "fs.hpp"


namespace ilrd
{
std::string IndentIMP(std::string str, size_t level);
bool CompFuncIMP(Entry *ent1, Entry* ent2);

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
    (void)level;

    return  Entry::GetName();
}

Directory::Directory(const std::string& name_)
    : Entry(name_)
{}

Directory::~Directory()
{}

std::string Directory::ToString(size_t level) const
{
    std::string res = Entry::GetName();
    
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
    else
    {
        throw(std::invalid_argument("no file was found"));
    }
    

    return *it;
}

Tree::Tree(std::string path)
{
    m_ent = ParseDirectory(path);
}

Tree::~Tree()
{
    std::cout << "size = \n" << m_allocations.size();
    for (size_t i = 0; i < m_allocations.size(); ++i)
    {
        delete(m_allocations[i]);
    }
    
    delete m_ent;
    std::cout << "\n\nDtor Tree\n";
}

Entry *Tree::GetEntry()
{
    return m_ent;
}

std::ostream& operator<<(std::ostream& os_, Tree& tree_)
{
    os_ << tree_.GetEntry()->ToString();

    return os_;
}

std::string IndentIMP(std::string str, size_t level)
{
    std::string res;
    for (size_t i = 0; i < level; ++i)
    {
        res += '\t';
    }

    return res + str;
}

bool CompFuncIMP(Entry *ent1, Entry* ent2)
{
    return (0 > strcmp(ent1->GetName().c_str(),
                       ent2->GetName().c_str()));
}

Entry *Tree::ParseDirectory(std::string path)
{
    size_t last_slash = path.rfind("/");
    std::string dir_name = path.substr(last_slash + 1);
    Directory *dir = new Directory(dir_name);
    DIR *p_dir = opendir(path.c_str());
    struct dirent *dir_ent = readdir(p_dir);

    while (NULL != dir_ent)
    {
        Entry *dir_entry = NULL;
        if ((0 != strcmp(dir_ent->d_name, ".")) && (0 != strcmp(dir_ent->d_name, "..")))
        {
            if (DT_DIR == dir_ent->d_type)
            {
                dir_entry = ParseDirectory(path + "/" + dir_ent->d_name);
            }
            else if (DT_REG == dir_ent->d_type)
            {
                dir_entry = new File(dir_ent->d_name);
            }

            ((Directory *)dir)->AddEntry(dir_entry);
            m_allocations.push_back(dir_entry);
        }

        dir_ent = readdir(p_dir);
    } 

    closedir(p_dir);

    return dir;
}

}
  

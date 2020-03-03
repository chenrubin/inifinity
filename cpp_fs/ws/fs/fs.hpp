// FS - Header file
// Last update: 01/03/2020
// Author: RD 78-79
/*----------------------------------------------------------------------------*/
#ifndef __FS_HPP__ 
#define __FS_HPP__
/*----------------------------------------------------------------------------*/
#include <string>
#include <vector>
/*----------------------------------------------------------------------------*/
namespace ilrd
{ 
// TODO: throw exceptions 
// make it uncopyable

class Entry
{
public:
	explicit Entry(const std::string& name_);
	virtual ~Entry();
	const std::string& GetName() const;
	virtual std::string ToString(size_t level = 0) const = 0;
private:
	std::string m_name;
};

class File : public Entry
{
public:
	explicit File(const std::string& name_);
	virtual ~File();
	virtual std::string ToString(size_t level = 0) const;
private:

};

class Directory : public Entry
{
public:
	explicit Directory(const std::string& name_);
	virtual ~Directory();
	virtual std::string ToString(size_t level = 0) const;
	void AddEntry(Entry* entry_);	
	Entry* RemoveEntry(Entry* entry_);

private:
	std::vector<Entry*> m_entries;
};

void ParseDirectory();

class Tree
{
public:
	explicit Tree(std::string path);
	~Tree();
	Entry *GetEntry();

private:
	std::string& m_path;
	Entry *m_ent;
	std::vector<Entry*> m_allocations;

	Entry *ParseDirectory(std::string path);
};

std::ostream& operator<<(std::ostream& os_, Tree& tree);

/*----------------------------------------------------------------------------*/
} // namespace ilrd
/*----------------------------------------------------------------------------*/
#endif // __FS_HPP__
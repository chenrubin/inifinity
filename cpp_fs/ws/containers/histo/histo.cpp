#include <cstring> /* string */
#include <map> /* multi map */
#include <queue> /* queue */
#include <iostream> /* cout */

#include "histo.hpp"

namespace std
{
const int default_value = 1;
static bool IsExistInMMap(const multimap<string,int>& mmap, string str);

void InsertStrings(multimap<string,int>& mmap, queue<string>& q, string str)
{
	if (!IsExistInMMap(mmap, str))
	{
		q.push(str);
	}
	
	mmap.insert(pair<string,int>(str,default_value));
}

void PrintStrings(multimap<string,int>& mmap, queue<string>& q)
{
	while (!q.empty())
	{
		string current_str = q.front();
		size_t count = mmap.count(current_str);
		if (1 == count)
		{
			cout << current_str << endl;
		}
		else
		{
			cout << count << " x " << current_str << endl;
		}
		
		q.pop();
	}
}

static bool IsExistInMMap(const multimap<string,int>& mmap, string str)
{
	return (0 != mmap.count(str));
}
}

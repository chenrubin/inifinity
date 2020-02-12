#include <cstring> /* string */
#include <queue> /* queue */
#include <map> /* multi map */
#include <iostream> /* cout */

#include "histo.hpp"

using namespace std;

int main()
{
	multimap<std::string,int> mmap;
	queue<std::string> q;
	string str = "";
	
	cout << "Please write several strings with enter between each" << endl;
	cout << "once you are finished write ." << endl;
	
	while (0 != str.compare("."))
	{
		cin >> str;
		if (0 != str.compare(".") )
		{
			InsertStrings(mmap, q, str);
		}	
	}
	
	PrintStrings(mmap, q);
}

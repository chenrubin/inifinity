#include <string.h>

class String
{
public:
	explicit String(char *str);
	explicit String& String(const String& other_);
	String& =operator(const String& other_);
	size_t Length();
	char *Cstr();
	bool opertaor==(const String& other);
	bool operator>(const String& other);
	bool operator<(const String& other);
	
	
private:
	char *m_str;	
}

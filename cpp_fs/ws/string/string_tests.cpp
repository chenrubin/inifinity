#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "string.hpp"
#include "MyUtils.h"


int main()
{
	ilrd::String str_default;
	PRINTTESTRESULTS("default String",1, 0 == strcmp(str_default.Cstr(), ""));
	ilrd::String str1 = "moshe";
	PRINTTESTRESULTS("String = str",2, 0 == strcmp(str1.Cstr(), "moshe"));
	ilrd::String str2("mishka");
	PRINTTESTRESULTS("String(str)",3, 0 == strcmp(str2.Cstr(), "mishka"));
	ilrd::String str3(str1);
	PRINTTESTRESULTS("string(string)",4, 0 == strcmp(str3.Cstr(), "moshe"));
	str3 = str2;
	PRINTTESTRESULTS("String = String",5, 0 == strcmp(str2.Cstr(), "mishka"));
	PRINTTESTRESULTS("String = String",6, 0 == strcmp(str3.Cstr(), "mishka"));
	PRINTTESTRESULTS("String = String",7, str3.Cstr() != str2.Cstr());
	
	PRINTTESTRESULTS("Length",8, 0 == str_default.Length());
	PRINTTESTRESULTS("Length",9, 5 == str1.Length());
	PRINTTESTRESULTS("Length",10, 6 == str2.Length());
	PRINTTESTRESULTS("Length",11, 6 == str3.Length());
	
	
	PRINTTESTRESULTS("==",12, 0 == (str_default == str1));
	PRINTTESTRESULTS("==",13, 0 == (str1 == str_default));
	PRINTTESTRESULTS("==",14, 1 == (str2 == str3));
	PRINTTESTRESULTS("==",15, 1 == (str3 == str2));
	PRINTTESTRESULTS("==",16, 0 == (str2 == str1));
	PRINTTESTRESULTS("==",17, 0 == (str1 == str2));
	
	PRINTTESTRESULTS("<",18, 0 == (str2 < str3));
	PRINTTESTRESULTS(">",19, 0 == (str3 > str2));
	PRINTTESTRESULTS("<",20, 0 == (str_default < str_default));
	PRINTTESTRESULTS(">",21, 1 == (str1 > str3));	

	return 0;
}

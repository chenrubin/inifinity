#include <stdio.h>
#include <iostream>
#include <cstring>

#include "rcstring.hpp"
#include "../../MyUtils.hpp"

using namespace ilrd;

void TestCtorCCtorDtor();
void TestOperators();
void TestConCat();
void TestCout();
void TestCin();

int main()
{
    TestCtorCCtorDtor();
    TestOperators();
    TestConCat();
    TestCout();
    TestCin();

    return 0;
}

void TestCtorCCtorDtor()
{
    RCString s1;
    RCString s2("moshe");
    RCString s3(s2);
}

void TestOperators()
{
    RCString s1;
    PRINTTESTRESULTS("TestOperators",1, 0 == strcmp("",s1.CStr()));
    RCString s2("moshe");
    PRINTTESTRESULTS("TestOperators",2, 0 == strcmp("",s1.CStr()));
    PRINTTESTRESULTS("TestOperators",3, 0 == strcmp("moshe",s2.CStr()));
    PRINTTESTRESULTS("TestOperators ==",4, s1 != s2);
    s1 = s2;
    PRINTTESTRESULTS("TestOperators ==",4, s1 == s2);
    PRINTTESTRESULTS("TestOperators =",5, 0 == strcmp("moshe",s1.CStr()));
    PRINTTESTRESULTS("TestOperators =",6, 0 == strcmp("moshe",s2.CStr()));
    RCString s3;
    s3 = "mishka";
    PRINTTESTRESULTS("TestOperators =",7, 0 == strcmp("mishka",s3.CStr()));
    s1[2] = 'f';
    PRINTTESTRESULTS("TestOperators []",8, 0 == strcmp("mofhe",s1.CStr()));
    PRINTTESTRESULTS("TestOperators []",9, 0 == strcmp("moshe",s2.CStr()));

    PRINTTESTRESULTS("TestOperators !=",10, s1 != s2);
    PRINTTESTRESULTS("TestOperators !=",11, s1 != "momo");    
    PRINTTESTRESULTS("TestOperators !=",12, s1 == "mofhe");

    PRINTTESTRESULTS("TestOperators >",13, s1 < "moshe");
    PRINTTESTRESULTS("TestOperators <",14, "mofhe" < s2);
    PRINTTESTRESULTS("TestOperators >",15, s1 < s2);
    PRINTTESTRESULTS("TestOperators <",16, s2 > s1);
    
    PRINTTESTRESULTS("TestOperators [] =",17, s1[2] == 'f');
    PRINTTESTRESULTS("TestOperators [] =",18, s1[3] == 'h');

    s1 = s1;
    PRINTTESTRESULTS("TestOperators = to self",19, 0 == strcmp("mofhe",s1.CStr()));
}

void TestConCat()
{
    RCString s1 = "moshe";
    RCString s2 = "lilach";
    PRINTTESTRESULTS("TestConCat length",1, 5 == s1.Length());
    PRINTTESTRESULTS("TestConCat length",2, 6 == s2.Length());

    s1.Concat(s2);
    PRINTTESTRESULTS("TestConCat length",3, 11 == s1.Length());
    PRINTTESTRESULTS("TestConCat length",4, 6 == s2.Length());
    PRINTTESTRESULTS("TestConCat",5, 0 == strcmp("moshelilach", s1.CStr()));
    PRINTTESTRESULTS("TestConCat",6, 0 == strcmp("lilach", s2.CStr()));
    s1.Concat(s1);
    PRINTTESTRESULTS("TestConCat length",7, 22 == s1.Length());
    PRINTTESTRESULTS("TestConCat",8, 0 == strcmp("moshelilachmoshelilach", s1.CStr()));
}

void TestCout()
{
    RCString s1 = "moshe";
    std::cout << s1;
    RCString s2 = "Rona";
    std::cout << s2;
    s1.Concat(s2);
    std::cout << s1;    
}

void TestCin()
{
    RCString s1;
    std::cout << "please enter string" << std::endl;
    std::cin >> s1;
    std::cout << "string is :" << s1 << std::endl;

    RCString s2 = "hello";
    RCString s3 = s2;
    std::cin >> s2;
    std::cout << "s2 string is :" << s2 << std::endl;
    std::cout << "s3 string is :" << s3 << std::endl;
}
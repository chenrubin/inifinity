#include <iostream> /* cout */

#include "fs.hpp"

using namespace ilrd;

int main()
{
    Directory main_dir("moshe");
    Directory dir1("mishka");
    Directory dir2("mishki");
    Directory dir3("ardan");
    File new_f1("file1.txt");
    File new_f2("file2.txt");
    File new_f3("file3.txt");
    File new_f4("file4.txt");
    File new_f5("file5.txt");

    main_dir.AddEntry(&new_f1);
    main_dir.AddEntry(&dir1);
    dir1.AddEntry(&new_f2);
    dir1.AddEntry(&new_f3);
    main_dir.AddEntry(&dir2);
    dir2.AddEntry(&new_f4);
    main_dir.AddEntry(&new_f5);
    main_dir.AddEntry(&dir3);

    std::string str = main_dir.ToString();    
    std::cout << str;

    std::cout << "\n!!!!!!!!!!!!!!!!!!\n"; 
    main_dir.RemoveEntry(&dir1);
    std::string str1 = main_dir.ToString();    
    std::cout << str1;
    std::cout << std::endl;

    return 0;
}


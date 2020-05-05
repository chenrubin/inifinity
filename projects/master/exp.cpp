#include <iostream>
#include <unistd.h>

int main()
{
    char buff[] = "moshe";
    std::cout << "before write\n";
    write(17, buff, 5);
    std::cout << "after write\n";
    
    return 0;
}
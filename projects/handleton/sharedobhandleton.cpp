#include <iostream> /* cout */

#include "handleton.hpp"

namespace ilrd
{
extern "C" void foo()
{
    int *sing = Singleton<int>::GetInstance();
    std::cout << "Shared object sing address = " << sing << std::endl;
}
}
#include <iostream> /* cout */

#include "singleton.hpp"

namespace ilrd
{
extern "C" void foo()
{
    int *sing = Singleton<int>::GetInstance();
    std::cout << "Shared object sing address = " << sing << std::endl;
}
}
#include <iostream> /* cout */

#include "singleton.hpp"


using namespace ilrd;

int main()
{
    Singleton<int> *single1 = Singleton<int>::GetInstance();
    Singleton<int> *single2 = Singleton<int>::GetInstance();
    Singleton<int> *single3 = Singleton<int>::GetInstance();
    

    return 0;
}
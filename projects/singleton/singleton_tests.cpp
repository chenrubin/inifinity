#include <iostream> /* cout */
#include <dlfcn.h> /* dlopen / dlsym */

#include "singleton.hpp"


using namespace ilrd;

int main()
{  

    int *single1 = Singleton<int>::GetInstance();
    
    std::cout << "main sing address = " << single1 << std::endl;

    void *handle = dlopen("./libsharedsingleton.so", RTLD_LAZY);
    if (!handle)
    {
        std::cout << "dlopen failed" << std::endl;
    }
    void (*func)();

    func = (void (*)())dlsym(handle, "foo");
    if (!func)
    {
         std::cout << "dlsym failed" << std::endl;
    }
    func();

    dlclose(handle);


    

    return 0;
}
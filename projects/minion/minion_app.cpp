#include "minion.hpp"

#define PORT /*12344*//*12345*/12346

using namespace ilrd;



int main()
{
    Minion minion(PORT);

    minion.Run();

 //   sleep(10);

//    minion.Stop();

    return 0;
}
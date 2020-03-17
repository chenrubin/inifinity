#include "bit_arr.hpp"

using namespace ilrd;

void TestCtorDtor();
void TestFlipAll();
void TestSetAll();
void TestAssignOrOperator();
void TestAssignAndOperator();
void TestAssignXorOperator();
void TestEqualityOperators();
void TestCountBits();
void TestConstBracketsOperator();
void TestBracketsOperator();
void TestShiftLeftOperator();
void TestShiftRightOperator();
void TestBoolOperator();

int main()
{
    TestCtorDtor();
    TestFlipAll();
    TestSetAll(); 
    TestAssignOrOperator();  
    TestAssignAndOperator();
    TestAssignXorOperator();
    TestEqualityOperators();
    TestCountBits();
    TestConstBracketsOperator();
    TestBracketsOperator();
    TestShiftLeftOperator();
    TestShiftRightOperator();
    TestBoolOperator();

    return 0;
}

void TestCtorDtor()
{
    BitArr<8> new_barr1;
    BitArr<9> new_barr2;
}

void TestFlipAll()
{
    BitArr<8> new_barr1;
    BitArr<9> new_barr2;
    PRINTTESTRESULTS("TestFlipAll",1, 0 == strcmp(new_barr1.ToString().c_str(), "0"));
    PRINTTESTRESULTS("TestFlipAll",2, 0 == strcmp(new_barr2.ToString().c_str(), "0"));

    new_barr1.FlipAll();
    new_barr2.FlipAll();
    PRINTTESTRESULTS("TestFlipAll",3, 0 == strcmp(new_barr1.ToString().c_str(), "ff"));
    PRINTTESTRESULTS("TestFlipAll",4, 0 == strcmp(new_barr2.ToString().c_str(), "1ff"));

    try
    {
       new_barr2.Flip(0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    PRINTTESTRESULTS("TestFlipAll",5, 0 == strcmp(new_barr2.ToString().c_str(), "1fe"));

    try
    {
       new_barr2.Flip(1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    PRINTTESTRESULTS("TestFlipAll",6, 0 == strcmp(new_barr2.ToString().c_str(), "1fc"));

    try
    {
       new_barr2.Flip(0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    PRINTTESTRESULTS("TestFlipAll",7, 0 == strcmp(new_barr2.ToString().c_str(), "1fd"));
}

void TestSetAll()
{
    BitArr<8> new_barr1;
    PRINTTESTRESULTS("TestSetAll",1, 0 == strcmp(new_barr1.ToString().c_str(), "0"));
    new_barr1.SetAll(1);
    PRINTTESTRESULTS("TestSetAll",2, 0 == strcmp(new_barr1.ToString().c_str(), "ff"));
    new_barr1.SetAll(0);
    PRINTTESTRESULTS("TestSetAll",3, 0 == strcmp(new_barr1.ToString().c_str(), "0"));
}

void TestAssignOrOperator()
{
    BitArr<9> new_barr1;
    BitArr<9> new_barr2;
    BitArr<13> new_barr3;

    new_barr1.Flip(0);
    new_barr1.Flip(2);
    new_barr1.Flip(4);
    new_barr1.Flip(6);

    new_barr2.Flip(0);
    new_barr2.Flip(1);
    new_barr2.Flip(3);

    new_barr1 |= new_barr2;
    PRINTTESTRESULTS("TestAssignOrOperator",1, 0 == strcmp(new_barr1.ToString().c_str(), "5f"));
    //   new_barr1 |= new_barr2; <-- this libe shouldn't compile

}

void TestAssignAndOperator()
{
    BitArr<9> new_barr1;
    BitArr<9> new_barr2;

    new_barr1.Flip(0);
    new_barr1.Flip(2);
    new_barr1.Flip(4);
    new_barr1.Flip(6);

    new_barr2.Flip(0);
    new_barr2.Flip(1);
    new_barr2.Flip(3);

    new_barr1 &= new_barr2;
    PRINTTESTRESULTS("TestAssignAndOperator",1, 0 == strcmp(new_barr1.ToString().c_str(), "1"));
    //   new_barr1 &= new_barr2; <-- this libe shouldn't compile
}

void TestAssignXorOperator()
{
    BitArr<9> new_barr1;
    BitArr<9> new_barr2;

    new_barr1.Flip(0);
    new_barr1.Flip(2);
    new_barr1.Flip(4);
    new_barr1.Flip(6);

    new_barr2.Flip(0);
    new_barr2.Flip(1);
    new_barr2.Flip(3);

    new_barr1 ^= new_barr2;
    PRINTTESTRESULTS("TestAssignXorOperator",1, 0 == strcmp(new_barr1.ToString().c_str(), "5e"));
    //   new_barr1 &= new_barr2; <-- this libe shouldn't compile
}

void TestEqualityOperators()
{
    BitArr<9> new_barr1;
    BitArr<9> new_barr2;

    new_barr1.Flip(0);
    new_barr1.Flip(2);
    new_barr1.Flip(4);
    new_barr1.Flip(6);

    new_barr2.Flip(0);
    new_barr2.Flip(2);
    new_barr2.Flip(4);
    new_barr2.Flip(6);

    PRINTTESTRESULTS("TestEqualityOperators",1, true == (new_barr1 == new_barr2));
    PRINTTESTRESULTS("TestEqualityOperators",2, false == (new_barr1 != new_barr2));

    new_barr2.Flip(0);

    PRINTTESTRESULTS("TestEqualityOperators",3, false == (new_barr1 == new_barr2));
    PRINTTESTRESULTS("TestEqualityOperators",4, true == (new_barr1 != new_barr2));
}

void TestCountBits()
{
    BitArr<9> new_barr1;

    PRINTTESTRESULTS("TestCountBits",1, 0 == new_barr1.Count(true));
    PRINTTESTRESULTS("TestCountBits",2, 9 == new_barr1.Count(false));
    PRINTTESTRESULTS("TestCountBits_ToString",3, 0 == strcmp(new_barr1.ToString().c_str(), "0"));
    new_barr1.Flip(0);
    PRINTTESTRESULTS("TestCountBits_ToString",3, 0 == strcmp(new_barr1.ToString().c_str(), "1"));
    PRINTTESTRESULTS("TestCountBits",3, 1 == new_barr1.Count(true));
    PRINTTESTRESULTS("TestCountBits",4, 8 == new_barr1.Count(false));
    new_barr1.Flip(2);
    PRINTTESTRESULTS("TestCountBits_ToString",3, 0 == strcmp(new_barr1.ToString().c_str(), "5"));
    PRINTTESTRESULTS("TestCountBits",5, 2 == new_barr1.Count(true));
    PRINTTESTRESULTS("TestCountBits",6, 7 == new_barr1.Count(false));
    new_barr1.Flip(4);
    PRINTTESTRESULTS("TestCountBits_ToString",3, 0 == strcmp(new_barr1.ToString().c_str(), "15"));
    PRINTTESTRESULTS("TestCountBits",7, 3 == new_barr1.Count(true));
    PRINTTESTRESULTS("TestCountBits",8, 6 == new_barr1.Count(false));
    new_barr1.Flip(6);
    PRINTTESTRESULTS("TestCountBits_ToString",3, 0 == strcmp(new_barr1.ToString().c_str(), "55"));
    PRINTTESTRESULTS("TestCountBits",9, 4 == new_barr1.Count(true));
    PRINTTESTRESULTS("TestCountBits",10, 5 == new_barr1.Count(false));

    new_barr1.Flip(8);
    std::cout << new_barr1.ToString();
    PRINTTESTRESULTS("TestCountBits_ToString",3, 0 == strcmp(new_barr1.ToString().c_str(), "155"));
}

void TestConstBracketsOperator()
{
    const BitArr<9> new_barr1;
    

    PRINTTESTRESULTS("TestConstBracketsOperator",1, 0 == new_barr1[0]);
    PRINTTESTRESULTS("TestConstBracketsOperator",2, 0 == new_barr1[0]);
    PRINTTESTRESULTS("TestConstBracketsOperator",3, 0 == new_barr1[0]);
    PRINTTESTRESULTS("TestConstBracketsOperator",4, 0 == new_barr1[0]);
    PRINTTESTRESULTS("TestConstBracketsOperator",5, 0 == new_barr1[0]);
    PRINTTESTRESULTS("TestConstBracketsOperator",6, 0 == new_barr1[0]);
    PRINTTESTRESULTS("TestConstBracketsOperator",7, 0 == new_barr1[0]);
    PRINTTESTRESULTS("TestConstBracketsOperator",8, 0 == new_barr1[0]);
    PRINTTESTRESULTS("TestConstBracketsOperator",9, 0 == new_barr1[0]);
}

void TestBracketsOperator()
{
    BitArr<65> new_barr1;
    
    new_barr1[0] = true;
    PRINTTESTRESULTS("TestBracketsOperator",1, 0 == strcmp(new_barr1.ToString().c_str(), "1"));
    new_barr1[10] = true;
    PRINTTESTRESULTS("TestBracketsOperator",1, 0 == strcmp(new_barr1.ToString().c_str(), "401"));
    new_barr1[20] = false;
    PRINTTESTRESULTS("TestBracketsOperator",1, 0 == strcmp(new_barr1.ToString().c_str(), "401"));
    new_barr1[45] = true;
}
  
void TestShiftLeftOperator()
{
    BitArr<9> new_barr1;
    BitArr<65> new_barr2;
    
    new_barr1[0] = true;
    new_barr1[1] = true;
    new_barr2[63] = true;
    PRINTTESTRESULTS("TestShiftLeftOperator",1, 0 == strcmp(new_barr1.ToString().c_str(), "3"));
    PRINTTESTRESULTS("TestShiftLeftOperator",2, 0 == strcmp(new_barr2.ToString().c_str(), "8000000000000000"));
    new_barr1 <<= 3;
    new_barr2 <<= 1;
    PRINTTESTRESULTS("TestShiftLeftOperator",3, 0 == strcmp(new_barr1.ToString().c_str(), "18"));
    PRINTTESTRESULTS("TestShiftLeftOperator",4, 0 == strcmp(new_barr2.ToString().c_str(), "10000000000000000"));
}

void TestShiftRightOperator()
{
    BitArr<9> new_barr1;
    BitArr<65> new_barr2;
    
    new_barr1[4] = true;
    new_barr1[5] = true;
    new_barr2[64] = true;
    PRINTTESTRESULTS("TestShiftRightOperator",1, 0 == strcmp(new_barr1.ToString().c_str(), "30"));
    PRINTTESTRESULTS("TestShiftRightOperator",2, 0 == strcmp(new_barr2.ToString().c_str(), "10000000000000000"));
    new_barr1 >>= 3;
    new_barr2 >>= 1;
    PRINTTESTRESULTS("TestShiftRightOperator",3, 0 == strcmp(new_barr1.ToString().c_str(), "6"));
    PRINTTESTRESULTS("TestShiftRightOperator",4, 0 == strcmp(new_barr2.ToString().c_str(), "8000000000000000"));
}

void TestBoolOperator()
{
    BitArr<9> new_barr1;
    new_barr1[0] = true;

    PRINTTESTRESULTS("TestBoolOperator",1, true == new_barr1[0]);
    PRINTTESTRESULTS("TestBoolOperator",2, false != new_barr1[0]);
    PRINTTESTRESULTS("TestBoolOperator",3, false == new_barr1[1]);
    PRINTTESTRESULTS("TestBoolOperator",4, true != new_barr1[1]);
}
#include <stdio.h>

#include "shared_ptr.hpp"
#include "MyUtils.hpp"

using namespace ilrd;

void TestCtorDtor();
void TestCCtor();
void TestArrowOperator();
void TestAssignmentOperator();
void TestDrivedBasetOperator();

struct moshe
{
	int a;
	int b;
};

class B
{
public:	
	virtual ~B(){}
};

class D : public B
{
public:	
	virtual ~D(){}
};

class C
{};

int main()
{
	TestCtorDtor();
	TestCCtor();
	TestArrowOperator();
	TestAssignmentOperator();
	TestDrivedBasetOperator();
	
	return 0;
}

void TestCtorDtor()
{
	int *p = new int(5);

	SharedPtr<int> new_sptr();
	SharedPtr<int> new_sptr1(p);
}

void TestCCtor()
{
	int *p = new int(5);
	int *t = new int(7);

	SharedPtr<int> new_sptr(p);
	SharedPtr<int> new_sptr1(t);

	PRINTTESTRESULTS("TestCCtor",1, 5 == *new_sptr);
	PRINTTESTRESULTS("TestCCtor",2, 7 == *new_sptr1);
}

void TestArrowOperator()
{
	moshe *m1 = new moshe;
	moshe *m2 = new moshe;

	m1->a = 1;
	m1->b = 2;
	m2->a = 3;
	m2->b = 4;

	SharedPtr<moshe> new_mosh1(m1);
	SharedPtr<moshe> new_mosh2(m2);

	PRINTTESTRESULTS("TestArrowOperator",1, 1 == new_mosh1->a);
	PRINTTESTRESULTS("TestArrowOperator",2, 2 == new_mosh1->b);
	PRINTTESTRESULTS("TestArrowOperator",3, 3 == new_mosh2->a);
	PRINTTESTRESULTS("TestArrowOperator",4, 4 == new_mosh2->b);
}

void TestAssignmentOperator()
{
	moshe *m1 = new moshe;
	moshe *m2 = new moshe;

	m1->a = 1;
	m1->b = 2;
	m2->a = 3;
	m2->b = 4;

	SharedPtr<moshe> new_mosh1(m1);
	SharedPtr<moshe> new_mosh2(m2);
	new_mosh2 = new_mosh1;
	new_mosh1 = new_mosh1;

	PRINTTESTRESULTS("TestAssignmentOperator",1, 1 == new_mosh1->a);
	PRINTTESTRESULTS("TestAssignmentOperator",2, 2 == new_mosh1->b);
	PRINTTESTRESULTS("TestAssignmentOperator",3, 1 == new_mosh2->a);
	PRINTTESTRESULTS("TestAssignmentOperator",4, 2 == new_mosh2->b);
}

void TestDrivedBasetOperator()
{
	B *m1 = new B;
	D *m2 = new D;

	SharedPtr<B> new_mosh1(m1);
	SharedPtr<D> new_mosh2(m2);
	new_mosh1 = new_mosh2;
//	new_mosh2 = new_mosh1;
}	C *m3 = new C;
#include <stdio.h>

#include "new_delete.h"
#include "MyUtils.h"

int main()
{
	Stack new_stack;
	
	PRINTTESTRESULTS("IsEmpty",1, 1 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",2, 0 == new_stack.Count());
	new_stack.Push(1);
	PRINTTESTRESULTS("IsEmpty",3, 0 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",4, 1 == new_stack.Count());
	PRINTTESTRESULTS("Top",5, 1 == new_stack.Top());
	new_stack.Push(2);
	PRINTTESTRESULTS("IsEmpty",6, 0 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",7, 2 == new_stack.Count());
	PRINTTESTRESULTS("Top",8, 2 == new_stack.Top());
	new_stack.Push(3);
	PRINTTESTRESULTS("IsEmpty",9, 0 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",10, 3 == new_stack.Count());
	PRINTTESTRESULTS("Top",11, 3 == new_stack.Top());
	new_stack.Push(4);
	PRINTTESTRESULTS("IsEmpty",12, 0 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",13, 4 == new_stack.Count());
	PRINTTESTRESULTS("Top",14, 4 == new_stack.Top());
	new_stack.Push(5);
	PRINTTESTRESULTS("IsEmpty",15, 0 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",16, 5 == new_stack.Count());
	PRINTTESTRESULTS("Top",17, 5 == new_stack.Top());
	
	new_stack.Pop();
	PRINTTESTRESULTS("IsEmpty",18, 0 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",19, 4 == new_stack.Count());
	PRINTTESTRESULTS("Top",20, 4 == new_stack.Top());
	new_stack.Pop();
	PRINTTESTRESULTS("IsEmpty",21, 0 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",22, 3 == new_stack.Count());
	PRINTTESTRESULTS("Top",23, 3 == new_stack.Top());
	new_stack.Pop();
	PRINTTESTRESULTS("IsEmpty",24, 0 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",25, 2 == new_stack.Count());
	PRINTTESTRESULTS("Top",26, 2 == new_stack.Top());
	new_stack.Pop();
	PRINTTESTRESULTS("IsEmpty",27, 0 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",28, 1 == new_stack.Count());
	PRINTTESTRESULTS("Top",29, 1 == new_stack.Top());
	new_stack.Pop();
	PRINTTESTRESULTS("IsEmpty",30, 1 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",31, 0 == new_stack.Count());
	
	new_stack.Push(1);
	PRINTTESTRESULTS("IsEmpty",32, 0 == new_stack.IsEmpty());
	PRINTTESTRESULTS("Count",33, 1 == new_stack.Count());
	PRINTTESTRESULTS("Top",34, 1 == new_stack.Top());
	new_stack.~Stack();
}

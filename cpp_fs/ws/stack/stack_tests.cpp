#include <stdio.h>

#include "stack.hpp"
#include "MyUtils.h"

int main()
{
	Stack<int> new_stack;
	
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
	
	/**/
	
	Stack<double> new_stack1;
	
	PRINTTESTRESULTS("IsEmpty",1, 1 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",2, 0 == new_stack1.Count());
	new_stack1.Push(1.2);
	PRINTTESTRESULTS("IsEmpty",3, 0 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",4, 1 == new_stack1.Count());
	PRINTTESTRESULTS("Top",5, 1.2 == new_stack1.Top());
	new_stack1.Push(2.5);
	PRINTTESTRESULTS("IsEmpty",6, 0 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",7, 2 == new_stack1.Count());
	PRINTTESTRESULTS("Top",8, 2.5 == new_stack1.Top());
	new_stack1.Push(3.6);
	PRINTTESTRESULTS("IsEmpty",9, 0 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",10, 3 == new_stack1.Count());
	PRINTTESTRESULTS("Top",11, 3.6 == new_stack1.Top());
	new_stack1.Push(4.6);
	PRINTTESTRESULTS("IsEmpty",12, 0 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",13, 4 == new_stack1.Count());
	PRINTTESTRESULTS("Top",14, 4.6 == new_stack1.Top());
	new_stack1.Push(5.75);
	PRINTTESTRESULTS("IsEmpty",15, 0 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",16, 5 == new_stack1.Count());
	PRINTTESTRESULTS("Top",17, 5.75 == new_stack1.Top());
	
	new_stack1.Pop();
	PRINTTESTRESULTS("IsEmpty",18, 0 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",19, 4 == new_stack1.Count());
	PRINTTESTRESULTS("Top",20, 4.6 == new_stack1.Top());
	new_stack1.Pop();
	PRINTTESTRESULTS("IsEmpty",21, 0 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",22, 3 == new_stack1.Count());
	PRINTTESTRESULTS("Top",23, 3.6 == new_stack1.Top());
	new_stack1.Pop();
	PRINTTESTRESULTS("IsEmpty",24, 0 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",25, 2 == new_stack1.Count());
	PRINTTESTRESULTS("Top",26, 2.5 == new_stack1.Top());
	new_stack1.Pop();
	PRINTTESTRESULTS("IsEmpty",27, 0 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",28, 1 == new_stack1.Count());
	PRINTTESTRESULTS("Top",29, 1.2 == new_stack1.Top());
	new_stack1.Pop();
	PRINTTESTRESULTS("IsEmpty",30, 1 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",31, 0 == new_stack1.Count());
	
	new_stack1.Push(1.56);
	PRINTTESTRESULTS("IsEmpty",32, 0 == new_stack1.IsEmpty());
	PRINTTESTRESULTS("Count",33, 1 == new_stack1.Count());
	PRINTTESTRESULTS("Top",34, 1.56 == new_stack1.Top());
	
	/**/
	
	Stack<ilrd::String> new_stack2;
	
	PRINTTESTRESULTS("IsEmpty",1, 1 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",2, 0 == new_stack2.Count());
	new_stack2.Push("hello");
	PRINTTESTRESULTS("IsEmpty",3, 0 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",4, 1 == new_stack2.Count());
	PRINTTESTRESULTS("Top",5, "hello" == new_stack2.Top());
	new_stack2.Push("mishka");
	PRINTTESTRESULTS("IsEmpty",6, 0 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",7, 2 == new_stack2.Count());
	PRINTTESTRESULTS("Top",8, "mishka" == new_stack2.Top());
	new_stack2.Push("popo");
	PRINTTESTRESULTS("IsEmpty",9, 0 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",10, 3 == new_stack2.Count());
	PRINTTESTRESULTS("Top",11, "popo" == new_stack2.Top());
	new_stack2.Push("tarkov");
	PRINTTESTRESULTS("IsEmpty",12, 0 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",13, 4 == new_stack2.Count());
	PRINTTESTRESULTS("Top",14, "tarkov" == new_stack2.Top());
	new_stack2.Push("zuzke");
	PRINTTESTRESULTS("IsEmpty",15, 0 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",16, 5 == new_stack2.Count());
	PRINTTESTRESULTS("Top",17, "zuzke" == new_stack2.Top());
	
	new_stack2.Pop();
	PRINTTESTRESULTS("IsEmpty",18, 0 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",19, 4 == new_stack2.Count());
	PRINTTESTRESULTS("Top",20, "tarkov" == new_stack2.Top());
	new_stack2.Pop();
	PRINTTESTRESULTS("IsEmpty",21, 0 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",22, 3 == new_stack2.Count());
	PRINTTESTRESULTS("Top",23, "popo" == new_stack2.Top());
	new_stack2.Pop();
	PRINTTESTRESULTS("IsEmpty",24, 0 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",25, 2 == new_stack2.Count());
	PRINTTESTRESULTS("Top",26, "mishka" == new_stack2.Top());
	new_stack2.Pop();
	PRINTTESTRESULTS("IsEmpty",27, 0 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",28, 1 == new_stack2.Count());
	PRINTTESTRESULTS("Top",29, "hello" == new_stack2.Top());
	new_stack2.Pop();
	PRINTTESTRESULTS("IsEmpty",30, 1 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",31, 0 == new_stack2.Count());
	
	new_stack2.Push("Sasha");
	PRINTTESTRESULTS("IsEmpty",32, 0 == new_stack2.IsEmpty());
	PRINTTESTRESULTS("Count",33, 1 == new_stack2.Count());
	PRINTTESTRESULTS("Top",34, "Sasha" == new_stack2.Top());
}

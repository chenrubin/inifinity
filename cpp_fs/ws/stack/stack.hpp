#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h> /*size_t*/
#include <stdio.h> /*printf*/
#include <assert.h> /*assert*/
#include "string.hpp"
#include "../../utils/utils.hpp"


template <typename Generic_Type>
class Stack
{
public:
		explicit Stack(); //default Ctor
		~Stack(); // Dtor
		
	// methods
	void Push(Generic_Type Data);
	void Pop();
	Generic_Type Top() const;
	size_t Count() const;
	bool IsEmpty() const;	

private:
	class Node
	{
		public:
			explicit Node(); //default Ctor
			explicit Node(const Generic_Type& data, Node* next); // regular Ctor

			Generic_Type GetData() const;
			bool HasNext() const;
			const Node &GetNext() const;
		
			const Generic_Type m_data;
			const Node *m_next;
	};

	Node m_dummy;
	Stack(const Stack& stck);
	Stack &operator=(const Stack& other_);
	
};

template <typename Generic_Type>
Stack<Generic_Type>::Node::Node()
	: m_data(), m_next(Deadbeef<Node>())
{
	printf("Default Node Ctor\n");
}

template <typename Generic_Type>
Stack<Generic_Type>::Node::Node(const Generic_Type& data, 
								Stack<Generic_Type>::Node *next)
	: m_data(data), m_next(next)
{
	printf("Default Node Ctor\n");
}

template <typename Generic_Type>
Generic_Type Stack<Generic_Type>::Node::GetData() const
{
	return m_data;
}

template <typename Generic_Type>
bool Stack<Generic_Type>::Node::HasNext() const
{
	return (Deadbeef<Node>() != m_next);
}

template <typename Generic_Type>	
const typename Stack<Generic_Type>::Node &Stack<Generic_Type>::Node::GetNext() const
{
	assert(HasNext());
	
	return *m_next;
}

template <typename Generic_Type>
Stack<Generic_Type>::Stack()
	: m_dummy()
{
	printf("Stack Ctor");
}

template <typename Generic_Type>
Stack<Generic_Type>::~Stack()
{	
	while (!IsEmpty())
	{
		Pop();
	}
}

template <typename Generic_Type>
void Stack<Generic_Type>::Push(Generic_Type Data)
{
	Stack::Node *new_node = new Node(Data, Deadbeef<Node>());
	
	if (m_dummy.HasNext())
	{
		new_node->m_next = &m_dummy.GetNext();
	}
	
	m_dummy.m_next = new_node;
}

template <typename Generic_Type>
void Stack<Generic_Type>::Pop()
{	
	const Node &temp_node = m_dummy.GetNext();
	
	if (m_dummy.GetNext().HasNext())
	{
		m_dummy.m_next = (const Node *)&m_dummy.GetNext().GetNext();
	}
	else
	{
		m_dummy.m_next = Deadbeef<Node>();
	}
	
	delete &temp_node;
}

template <typename Generic_Type>
size_t Stack<Generic_Type>::Count() const
{
	Node *runner = (Node *)&m_dummy;
	size_t counter = 0;
	
	while (runner->HasNext())
	{
		++counter;
		runner = (Node *)&(runner->GetNext());
	}
	
	return counter;
}

template <typename Generic_Type>
Generic_Type Stack<Generic_Type>::Top() const
{
	return (m_dummy.m_next->m_data);
}

template <typename Generic_Type>
bool Stack<Generic_Type>::IsEmpty() const
{
	return (!(m_dummy.HasNext()));
}
#endif

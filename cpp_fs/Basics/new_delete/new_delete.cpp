#include <stdio.h> /*printf*/
#include <assert.h> /* assert */

#include "new_delete.h"


Stack::Node::Node()
	: m_data(0), m_next(NULL)
{
	printf("Default Node Ctor\n");
}

Stack::Node::Node(int m_int, Stack::Node *next)
	: m_data(m_int), m_next(next)
{
	printf("Default Node Ctor\n");
}

DataType Stack::Node::GetData() const
{
	return m_data;
}

bool Stack::Node::HasNext() const
{
	return (NULL != m_next);
}
	
const Stack::Node &Stack::Node::GetNext() const
{
	assert(HasNext());
	
	return *m_next;
}

Stack::Stack()
	: m_dummy(0,NULL)
{
	printf("Stack Ctor");
}

Stack::~Stack()
{	
	while (!IsEmpty())
	{
		Pop();
	}
}

void Stack::Push(DataType Data)
{
	Stack::Node *new_node = new Node(Data, NULL);
	
	if (m_dummy.HasNext())
	{
		new_node->m_next = &m_dummy.GetNext();
	}
	
	m_dummy.m_next = new_node;
}

void Stack::Pop()
{	
	const Node &temp_node = m_dummy.GetNext();
	
	if (m_dummy.GetNext().HasNext())
	{
		m_dummy.m_next = (const Node *)&m_dummy.GetNext().GetNext();
	}
	else
	{
		m_dummy.m_next = NULL;
	}
	
	delete &temp_node;
}

size_t Stack::Count() const
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

DataType Stack::Top() const
{
	return (m_dummy.m_next->m_data);
}

bool Stack::IsEmpty() const
{
	return (!(m_dummy.HasNext()));
}

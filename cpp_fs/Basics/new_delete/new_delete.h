#include <stddef.h> /*size_t*/
#include <stdio.h> /*printf*/

typedef int DataType;

struct Stack
{
	public:
		explicit Stack(); //default Ctor
		~Stack(); // Dtor
		
	// methods	
	void Push(DataType Data);
	void Pop();
	DataType Top() const;
	size_t Count() const;
	bool IsEmpty() const;	

	struct Node
	{
		explicit Node(); //default Ctor
		explicit Node(DataType data, Node* next); // regular Ctor

		DataType GetData() const;
		bool HasNext() const;
		const Node &GetNext() const;

		const DataType m_data;
		const Node *m_next;
	};

	Node m_dummy;
};

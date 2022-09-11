#pragma once

#include "ObjectPool.h"

template<typename T>
class Stack final
{
	struct Node
	{
		T Data;
		Node* Next;
	};
public:
	Stack(unsigned int capacity);
	~Stack();

	void	Push(T data);
	T		Pop();
private:
	ObjectPool<Node>	mPool;
	Node*				mTop;
	unsigned int		mCapacity;
};

template<typename T>
inline Stack<T>::Stack(unsigned int capacity)
	:	mPool(capacity),
		mTop(nullptr),
		mCapacity(capacity)
{
}

template<typename T>
inline Stack<T>::~Stack()
{
	Node* cur;
	Node* next;
	
	cur = mTop;
	while (cur != nullptr)
	{
		next = cur->Next;
		mPool.ReleaseObject(cur);
		cur = next;
	}
}

template<typename T>
inline void Stack<T>::Push(T data)
{
	Node* newTop = mPool.GetObject();
	Node* oldTop;

	newTop->Data = data;

	while (true)
	{
		oldTop = mTop;
		newTop->Next = REMOVE_OP_COUNT_FROM(oldTop);

		if (InterlockedCompareExchangePointer((PVOID*)&mTop, MAKE_TOP(newTop, EXTRACT_OP_COUNT_FROM(oldTop) + 1), oldTop) == oldTop)
		{
			break;
		}
	}
}

template<typename T>
inline T Stack<T>::Pop()
{
	Node* oldTop;
	Node* newTop;
	 // 성능 하락지점 있다...
	while (true)
	{
		oldTop = mTop;
		newTop = MAKE_TOP(REMOVE_OP_COUNT_FROM(oldTop)->Next, EXTRACT_OP_COUNT_FROM(oldTop) + 1);

		if (InterlockedCompareExchangePointer((PVOID*)&mTop, newTop, oldTop) == oldTop)
		{
			break;
		}
	}
	oldTop = REMOVE_OP_COUNT_FROM(oldTop);

	T data = oldTop->Data;

	mPool.ReleaseObject(oldTop);

	return data;
}
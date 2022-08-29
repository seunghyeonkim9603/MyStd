#pragma once

template<typename T>
class Stack final
{
public:
	struct Node
	{
		T		Data;
		Node*	Next;
	};
public:
	Stack();
	~Stack();
	Stack(Stack<T>& other);

	Stack<T>& operator=(Stack<T>& other);

	void			Push(T data);
	T				Pop();
	bool			IsEmpty() const;
	unsigned int	GetSize() const;

private:
	Node* mTop;
	unsigned int mSize;
};

template<typename T>
inline Stack<T>::Stack()
	:	mTop(nullptr),
		mSize(0)
{
}

template<typename T>
inline Stack<T>::~Stack()
{
	Node* cur;

	cur = mTop;
	while (cur != nullptr)
	{
		Node* next = cur->Next;
		delete cur;
		cur = next;
	}
}

template<typename T>
inline Stack<T>::Stack(Stack<T>& other)
	: mTop(other.mTop)
	mCapacity(other.mCapacity)
{
	mStack = new T[mCapacity];
	memcpy(mStack, other.mStack, sizeof(T) * mCapacity);
}

template<typename T>
inline Stack<T>& Stack<T>::operator=(Stack<T>& other)
{
	if (mStack != nullptr)
	{
		delete[] mStack;
	}
	mStack = new T[other.mCapacity];
	mCapacity = other.mCapacity;
	mTop = other.mTop;

	memcpy(mStack, other.mStack, sizeof(T) * mCapacity);

	return *this;
}

template<typename T>
inline void Stack<T>::Push(T val)
{
	if (mTop + 1 == mCapacity)
	{
		Reserve(mCapacity * 2);
	}
	mStack[++mTop] = val;
}

template<typename T>
inline T Stack<T>::Pop()
{
	if (mTop == -1)
	{

	}
	return T();
}

template<typename T>
inline bool Stack<T>::IsEmpty()
{
	return false;
}

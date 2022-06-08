#pragma once

#include <memory>

template<typename T>
class Stack final
{
public:
	Stack();
	Stack(size_t capacity);
	~Stack();
	Stack(Stack<T>& other);
	
	Stack<T>& operator=(Stack<T>& other);

	void Push(T val);
	T Pop();
	void Reserve(size_t newCapacity);
	bool IsEmpty();

private:
	enum { DEFAULT_SIZE = 256 };
	T* mStack;
	size_t mCapacity;
	int mTop;
};

template<typename T>
inline Stack<T>::Stack()
	: Stack(DEFAULT_SIZE)
{
}

template<typename T>
inline Stack<T>::Stack(size_t capacity)
	: mTop(-1),
	mCapacity(capacity)
	mStack(new T[capacity])
{
}

template<typename T>
inline Stack<T>::~Stack()
{
	delete[] mStack;
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
inline void Stack<T>::Reserve(size_t newCapacity)
{
}

template<typename T>
inline bool Stack<T>::IsEmpty()
{
	return false;
}

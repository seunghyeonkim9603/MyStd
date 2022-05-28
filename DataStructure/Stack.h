#pragma once

template<typename T>
class Stack final
{
public:
	Stack();
	Stack(size_t capacity);
	~Stack();
	Stack(Stack& other);
	
	Stack& operator=(Stack& other);

	void Push(T val);
	T Pop();
	void Reserve(size_t newCapacity);
	bool IsEmpty();
private:
	T* mStack;
	size_t mTop;
};
#pragma once

template <typename T>
class LinkedList final
{
private:
	struct Node
	{
		T Data;
		Node* Next;
		Node* Prev;
	};

public:
	class Iterator
	{
	public:
		Iterator(Node* node);
		~Iterator() = default;

		Iterator& operator++(void);
		const Iterator& operator++(int);
		Iterator& operator--(void);
		const Iterator& operator--(int);
		T operator*(void) const;
		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;

	private:
		Node* mNode;
	};

public:
	LinkedList(void);
	~LinkedList(void);

	void AddFirst(T data);
	void AddLast(T data);
	bool TryRemoveFirst(void);
	bool TryRemoveLast(void);
	void Clear(void);

	Iterator Begin(void);
	Iterator End(void);
	Iterator Erase(Iterator iter);
private:

	Node mFront;
	Node mRear;
	size_t mSize;
};

template<typename T>
LinkedList<T>::Iterator::Iterator(Node* node)
	:mNode(node)
{
}

template<typename T>
typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++(void)
{
	mNode = mNode->Next;

	return *this;
}

template<typename T>
typename const LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++(int)
{
	const Iterator& iter = this;
	++this;

	return iter;
}

template<typename T>
typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator--(void)
{
	mNode = mNode->Prev;

	return *this;
}

template<typename T>
typename const LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator--(int)
{
	const Iterator& iter = this;
	--this;

	return iter;
}

template<typename T>
T LinkedList<T>::Iterator::operator*(void) const
{
	return mNode->Data;
}

template<typename T>
bool LinkedList<T>::Iterator::operator==(const LinkedList<T>::Iterator& other) const
{
	if (mNode == other.mNode)
	{
		return true;
	}
	return false;
}

template<typename T>
bool LinkedList<T>::Iterator::operator!=(const LinkedList<T>::Iterator& other) const
{
	if (mNode != other.mNode)
	{
		return true;
	}
	return false;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::Begin(void)
{
	LinkedList<T>::Iterator iter(mFront.Next);

	return iter;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::End(void)
{
	LinkedList<T>::Iterator iter(&mRear);

	return iter;
}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::Erase(LinkedList<T>::Iterator iter)
{
	Node* delNode = iter.mNode;
	LinkedList<T>::Iterator nextIter(delNode->Next);

	delNode->Prev->Next = delNode->Next;
	delNode->Next->Prev = delNode->Prev;
	--mSize;

	delete delNode;

	return nextIter;
}

template<typename T>
LinkedList<T>::LinkedList(void)
	: mSize(0)
{
	mFront.Next = &mRear;
	mFront.Prev = nullptr;
	mRear.Prev = &mFront;
	mRear.Next = nullptr;
}

template<typename T>
LinkedList<T>::~LinkedList(void)
{
	Node* delNode = mFront.Next;

	while (delNode != &mRear)
	{
		Node* nextNode = delNode->Next;
		delete delNode;

		delNode = nextNode;
	}
}

template<typename T>
void LinkedList<T>::AddFirst(T data)
{
	Node* newNode = new Node;

	newNode->Data = data;
	newNode->Prev = &mFront;
	newNode->Next = mFront.Next;

	mFront.Next->Prev = newNode;
	mFront.Next = newNode;
	++mSize;
}

template<typename T>
void LinkedList<T>::AddLast(T data)
{
	Node* newNode = new Node;
	
	newNode->Data = data;
	newNode->Next = &mRear;
	newNode->Prev = mRear.Prev;

	mRear.Prev->Next = newNode;
	mRear.Prev = newNode;
	++mSize;
}

template<typename T>
bool LinkedList<T>::TryRemoveFirst(void)
{
	if (mSize == 0)
	{
		return false;
	}
	Node* delNode = mFront.Next;

	mFront.Next = delNode->Next;
	delNode->Next->Prev = &mFront;
	--mSize;

	delete delNode;

	return true;
}

template<typename T>
bool LinkedList<T>::TryRemoveLast(void)
{
	if (mSize == 0)
	{
		return false;
	}
	Node* delNode = mRear.Prev;

	mRear.Prev = delNode->Prev;
	delNode->Prev->Next = &mRear;
	--mSize;

	delete delNode;

	return true;
}

template<typename T>
void LinkedList<T>::Clear(void)
{
	Node* delNode = mFront.Next;

	while (delNode != &mRear)
	{
		Node* nextNode = delNode->Next;
		delete delNode;

		delNode = nextNode;
	}
	mFront.Next = &mRear;
	mRear.Prev = &mFront;
	mSize = 0;
}
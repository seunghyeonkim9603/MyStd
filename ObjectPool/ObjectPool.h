#pragma once

#define MAKE_TOP(ptr, op_count) reinterpret_cast<Node*>((ptr) | ((op_count) << 48))
#define REMOVE_OP_COUNT_FROM(ptr) reinterpret_cast<Node*>((ptr) & 0x00007FFFFFFFFFFF)
#define EXTRACT_OP_COUNT_FROM(ptr) reinterpret_cast<unsigned short>((ptr) >> 48)

template<typename T>
class ObjectPool final
{
public:
	struct Node
	{
		T*		Obj;
		Node*	Next;
	};

public:
	ObjectPool(size_t size, bool bIsPlacementNew);
	ObjectPool(ObjectPool& other) = delete;
	ObjectPool(ObjectPool&& other) = delete;
	~ObjectPool();

	template<typename ...TArgs>
	T*		GetObject(TArgs&&... args);
	void	ReleaseObject(T* obj);

	size_t GetAllCount() const;
	size_t GetActiveCount() const;
	size_t GetInactiveCount() const;

private:
	Node*	mTop;
	size_t	mAllCount;
	size_t	mActiveCount;
	bool	mbIsPlacementNew;
};

template<typename T>
inline ObjectPool<T>::ObjectPool(size_t size, bool bIsPlacementNew)
	:	mTop(nullptr),
		mAllCount(size),
		mActiveCount(0),
		mbIsPlacementNew(bIsPlacementNew)
{
	if (bIsPlacementNew)
	{
		for (size_t i = 0; i < size; ++i)
		{
			Node* newNode = new Node();
			{
				newNode->Obj = reinterpret_cast<char*>(new char[sizeof(T)]);
				newNode->Next = mTop;
			}
			mTop = newNode;
		}
	}
	else
	{
		for (size_t i = 0; i < size; ++i)
		{
			Node* newNode = new Node();
			{
				newNode->Obj = new T();
				newNode->Next = mTop;
			}
			mTop = newNode;
		}
	}
}

template<typename T>
inline ObjectPool<T>::~ObjectPool()
{
	Node* cur = REMOVE_OP_COUNT_FROM(mTop);
	Node* next;

	if (mbIsPlacementNew)
	{
		while (cur != nullptr)
		{
			next = cur->Next;
			delete reinterpret_cast<char*>(cur->Obj);
			delete cur;
			cur = next;
		}
	}
	else
	{
		for (int i = 0; i < mPool.size(); ++i)
		{
			next = cur->Next;
			delete cur->Obj;
			delete cur;
			cur = next;
		}
	}
}

template<typename T>
template<typename ...TArgs>
inline T* ObjectPool<T>::GetObject(TArgs&&... args)
{
	if (mTop == nullptr)
	{
		InterlockedIncrement64(&mAllCount);

		Node* newNode = new Node();
		{
			newNode->Obj = new T(std::forward<TArgs>(args)...);
		}
		return reinterpret_cast<T*>(newNode);
	}

	Node* oldTop;
	Node* newTop;

	while (true)
	{
		oldTop = mTop;
		unsigned short opCount = EXTRACT_OP_COUNT_FROM(oldTop);
		newTop = MAKE_TOP(oldTop->Next, opCount + 1);

		if (InterlockedCompareExchangePointer(&mTop, newTop, oldTop) == oldTop)
		{
			break;
		}
	}
	oldTop = REMOVE_OP_COUNT_FROM(oldTop);

	if (mbIsPlacementNew)
	{
		new (oldTop->Obj) T(std::forward<TArgs>(args)...);
	}
	InterlockedIncrement64(&mActiveCount);

	return reinterpret_cast<T*>oldTop;
}

template<typename T>
inline void ObjectPool<T>::ReleaseObject(T* obj)
{
	if (mbIsPlacementNew)
	{
		obj->T::~T();
	}
	Node* oldTop;
	Node* newTop = reinterpret_cast<Node*>(obj);

	while (true)
	{
		oldTop = mTop;
		newTop = reinterpret_cast<Node*>(obj);

		unsigned short opCount = EXTRACT_OP_COUNT_FROM(oldTop);

		newTop->Next = REMOVE_OP_COUNT_FROM(oldTop);
		newTop = MAKE_TOP(oldTop, opCount + 1);

		if (InterlockedCompareExchangePointer(&mTop, newTop, oldTop) == oldTop)
		{
			break;
		}
	}
	InterlockedDecrement64(&mActiveCount);
}


template<typename T>
inline size_t ObjectPool<T>::GetAllCount() const
{
	return mAllCount;
}

template<typename T>
inline size_t ObjectPool<T>::GetActiveCount() const
{
	return mActiveCount;
}

template<typename T>
inline size_t ObjectPool<T>::GetInactiveCount() const
{
	return mAllCount - mActiveCount;
}

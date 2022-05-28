#pragma once

#include <stack>

template<typename T>
class ObjectPool final
{
public:
	ObjectPool(size_t size);
	ObjectPool(ObjectPool& other) = delete;
	ObjectPool(ObjectPool&& other) = delete;
	~ObjectPool();

	template<typename ...TArgs>
	T* GetObject(TArgs&&... args);

	void ReleaseObject(T* obj);

	void GetAllCount() const;
	void GetActiveCount() const;
	void GetInactiveCount() const;

private:
	std::stack<char*> mPool;
	size_t mAllCount;
	size_t mActiveCount;
};

template<typename T>
inline ObjectPool<T>::ObjectPool(size_t size)
	: mAllCount(size),
	mActiveCount(0)
{
	mPool.

	for (size_t i = 0; i < size; ++i)
	{
		mPool.push(new char[sizeof(T)]);
	}
}

template<typename T>
inline ObjectPool<T>::~ObjectPool()
{
	for (int i = 0; i < mPool.size(); ++i)
	{
		delete mPool[i];
	}
}

template<typename T>
template<typename ...TArgs>
inline T* ObjectPool<T>::GetObject(TArgs&&... args)
{
	++mActiveCount;

	if (mPool.empty())
	{
		++mAllCount;
		return new T(std::forward<TArgs>(args)...);
	}
	T* obj = reinterpret_cast<T*>(mPool.top());
	mPool.pop();

	obj = new (obj) T(std::forward<TArgs>(args)...);

	return obj;
}

template<typename T>
inline void ObjectPool<T>::ReleaseObject(T* obj)
{
	obj->T::~T();

	mPool.push(reinterpret_cast<char*>(obj));
	--mActiveCount;
}

template<typename T>
inline void ObjectPool<T>::GetAllCount() const
{
	return mAllCount;
}

template<typename T>
inline void ObjectPool<T>::GetActiveCount() const
{
	return mActiveCount;
}

template<typename T>
inline void ObjectPool<T>::GetInactiveCount() const
{
	return mAllCount - mActiveCount;
}

#pragma once

#include <vector>

template<typename T>
class ObjectPool final
{
public:
	ObjectPool(size_t size, bool bIsPlacementNew);
	ObjectPool(ObjectPool& other) = delete;
	ObjectPool(ObjectPool&& other) = delete;
	~ObjectPool();

	template<typename ...TArgs>
	T* GetObject(TArgs&&... args);

	void ReleaseObject(T* obj);

	size_t GetAllCount() const;
	size_t GetActiveCount() const;
	size_t GetInactiveCount() const;

private:
	std::vector<char*> mPool;
	size_t mAllCount;
	size_t mActiveCount;
	bool mbIsPlacementNew;
};

template<typename T>
inline ObjectPool<T>::ObjectPool(size_t size, bool bIsPlacementNew)
	: mAllCount(size),
	mActiveCount(0),
	mbIsPlacementNew(bIsPlacementNew)
{
	mPool.reserve(size);

	if (bIsPlacementNew)
	{
		for (size_t i = 0; i < size; ++i)
		{
			mPool.push_back(new char[sizeof(T)]);
		}
	}
	else
	{
		for (size_t i = 0; i < size; ++i)
		{
			mPool.push_back(reinterpret_cast<char*>(new T()));
		}
	}
}

template<typename T>
inline ObjectPool<T>::~ObjectPool()
{
	if (mbIsPlacementNew)
	{
		for (int i = 0; i < mPool.size(); ++i)
		{
			delete mPool[i];
		}
	}
	else
	{
		for (int i = 0; i < mPool.size(); ++i)
		{
			T* obj = reinterpret_cast<T*>(mPool[i]);

			delete obj;
		}
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
	T* obj = reinterpret_cast<T*>(mPool.back());
	mPool.pop_back();

	if (mbIsPlacementNew)
	{
		obj = new (obj) T(std::forward<TArgs>(args)...);
	}
	return obj;
}

template<typename T>
inline void ObjectPool<T>::ReleaseObject(T* obj)
{
	if (mbIsPlacementNew)
	{
		obj->T::~T();
	}
	mPool.push_back(reinterpret_cast<char*>(obj));
	--mActiveCount;
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

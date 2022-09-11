#pragma once

#include "ObjectPool.h"

template<typename T>
class Queue
{
private:
	struct Node
	{
		T Data;
		Node* Next;
	};

	struct MemoryLog
	{
		DWORD ThreadID;
		char Op;
		Node* Temp;
		Node* Next;
		char Unused[8];
	};

public:
	Queue(unsigned int capacity)
		: mPool(capacity),
		mSize(0),
		mHead(nullptr),
		mTail(nullptr),
		mLogIndex(0)
	{
		mHead = mPool.GetObject();
		mTail = mHead;
		mHead->Next = nullptr;
	}

	void Enqueue(T t, DWORD threadID)
	{
		Node* newNode = mPool.GetObject();

		newNode->Data = t;
		newNode->Next = nullptr;

		while (true)
		{
			Node* tail = mTail;
			Node* next = REMOVE_OP_COUNT_FROM(tail)->Next;

			if (next == nullptr)
			{
				if (InterlockedCompareExchangePointer((PVOID*)&REMOVE_OP_COUNT_FROM(tail)->Next, newNode, next) == next)
				{
					MemoryLog* log = &mLogs[InterlockedIncrement64(&mLogIndex) % LOG_SIZE];
					log->ThreadID = threadID;
					log->Op = 1;
					log->Temp = tail;
					log->Next = newNode;

					InterlockedCompareExchangePointer((PVOID*)&mTail, MAKE_TOP(newNode, EXTRACT_OP_COUNT_FROM(tail) + 1), tail);
					break;
				}
			}
			else
			{
				InterlockedCompareExchangePointer((PVOID*)&mTail, MAKE_TOP(next, EXTRACT_OP_COUNT_FROM(tail) + 1), tail);
			}
		}
		InterlockedIncrement(&mSize);
	}

	bool TryDequeue(T* t, DWORD threadID)
	{
		if (InterlockedDecrement(&mSize) == -1)
		{
			InterlockedIncrement(&mSize);
			return false;
		}

		while (true)
		{
			Node* head = mHead;
			Node* tail = mTail;
			Node* next = REMOVE_OP_COUNT_FROM(head)->Next;

			if (REMOVE_OP_COUNT_FROM(head) == REMOVE_OP_COUNT_FROM(tail))
			{
				InterlockedCompareExchangePointer((PVOID*)&mTail, MAKE_TOP(next, EXTRACT_OP_COUNT_FROM(tail) + 1), tail);
			}
			if (InterlockedCompareExchangePointer((PVOID*)&mHead, MAKE_TOP(next, EXTRACT_OP_COUNT_FROM(head) + 1), head) == head)
			{
				MemoryLog* log = &mLogs[InterlockedIncrement64(&mLogIndex) % LOG_SIZE];
				log->ThreadID = threadID;
				log->Op = 2;
				log->Temp = REMOVE_OP_COUNT_FROM(head);
				log->Next = next;

				*t = next->Data;
				head = REMOVE_OP_COUNT_FROM(head);
				mPool.ReleaseObject(head);
				break;
			}
		}
		return true;
	}

	bool SleepDequeue(T* t, DWORD threadID)
	{
		if (InterlockedDecrement(&mSize) == -1)
		{
			InterlockedIncrement(&mSize);
			return false;
		}

		while (true)
		{
			Node* head = mHead;
			Node* tail = mTail;
			Node* next = REMOVE_OP_COUNT_FROM(head)->Next;

			if (next != nullptr)
			{
				if (REMOVE_OP_COUNT_FROM(head) == tail)
				{
					InterlockedCompareExchangePointer((PVOID*)&mTail, next, tail);
				}
				if (InterlockedCompareExchangePointer((PVOID*)&mHead, MAKE_TOP(next, EXTRACT_OP_COUNT_FROM(head) + 1), head) == head)
				{
					MemoryLog* log = &mLogs[InterlockedIncrement64(&mLogIndex) % LOG_SIZE];
					log->ThreadID = threadID;
					log->Op = 2;
					log->Temp = REMOVE_OP_COUNT_FROM(head);
					log->Next = next;

					*t = next->Data;
					head = REMOVE_OP_COUNT_FROM(head);
					mPool.ReleaseObject(head);
					break;
				}
			}
		}
		return true;
	}

private:
	enum { LOG_SIZE = 10000 };

	ObjectPool<Node> mPool;
	long mSize;

	Node* mHead;
	Node* mTail;

	MemoryLog mLogs[LOG_SIZE];
	long long mLogIndex;
};
#include <memory>

#include "RingBuffer.h"

RingBuffer::RingBuffer()
	:RingBuffer(DEFAULT_SIZE)
{
}

RingBuffer::RingBuffer(const int capacity)
	:mBuffer(new char[capacity + 1]),
	mCapacity(capacity + 1)
{
	mFront = 0;
	mRear = 0;
}

RingBuffer::~RingBuffer()
{
	delete mBuffer;
}

bool RingBuffer::TryEnqueue(const char* data, const int length)
{
	if (GetUnusedSize() < length)
	{
		return false;
	}

	if (mRear < mFront)
	{
		memcpy(&mBuffer[mRear], data, length);
	}
	else
	{
		const int directEnqueueableSize = GetDirectEnqueueableSize();

		if (length <= directEnqueueableSize)
		{
			memcpy(&mBuffer[mRear], data, length);
		}
		else
		{
			memcpy(&mBuffer[mRear], data, directEnqueueableSize);
			memcpy(&mBuffer[0], &data[directEnqueueableSize], length - directEnqueueableSize);
		}
	}
	MoveRear(length);

	return true;
}

bool RingBuffer::TryDequeue(char* outDest, const int length)
{
	if (!TryPeek(outDest, length))
	{
		return false;
	}
	MoveFront(length);

	return true;
}

bool RingBuffer::TryPeek(char* outDest, const int length) const
{
	if (GetSize() < length)
	{
		return false;
	}

	if (mFront < mRear)
	{
		memcpy(outDest, &mBuffer[mFront], length);
	}
	else
	{
		const int directDequeueableSize = GetDirectDequeueableSize();

		if (length <= directDequeueableSize)
		{
			memcpy(outDest, &mBuffer[mFront], length);
		}
		else
		{
			memcpy(outDest, &mBuffer[mFront], directDequeueableSize);
			memcpy(&outDest[directDequeueableSize], &mBuffer[0], length - directDequeueableSize);
		}
	}
	return true;
}

void RingBuffer::Reserve(const int newCapacity)
{
	if (newCapacity <= mCapacity)
	{
		return;
	}
	RingBuffer temp(newCapacity);

	TryDequeue(temp.mBuffer, GetSize());

	*this = std::move(temp);
}

bool RingBuffer::IsEmpty()
{
	return mFront == mRear;
}

bool RingBuffer::IsFull()
{
	if (mFront == (mRear + 1) % mCapacity)
	{
		return true;
	}
	return false;
}

void RingBuffer::Clear()
{
	mFront = mRear;
}

int RingBuffer::GetCapacity() const
{
	return mCapacity - 1;
}

int RingBuffer::GetSize() const
{
	if (mRear < mFront)
	{
		return mCapacity - mFront + mRear;
	}
	return mRear - mFront;
}

int RingBuffer::GetUnusedSize() const
{
	return mCapacity - GetSize() - 1;
}

int RingBuffer::GetDirectEnqueueableSize() const
{
	if (mRear < mFront)
	{
		return mFront - (mRear + 1);
	}
	if (mFront == 0)
	{
		return mCapacity - mRear - 1;
	}
	return mCapacity - mRear;
}

int RingBuffer::GetDirectDequeueableSize() const
{
	if (mRear < mFront)
	{
		return mCapacity - mFront;
	}
	return mRear - mFront;
}


char* RingBuffer::GetFront() const
{
	return &mBuffer[mFront];
}

char* RingBuffer::GetRear() const
{
	return &mBuffer[mRear];
}

void RingBuffer::MoveFront(const int offset)
{
	mFront = (mFront + offset) % mCapacity;
}

void RingBuffer::MoveRear(const int offset)
{
	mRear = (mRear + offset) % mCapacity;
}

RingBuffer& RingBuffer::operator=(RingBuffer&& other) noexcept
{
	delete mBuffer;

	mBuffer = other.mBuffer;
	mFront = other.mFront;
	mRear = other.mRear;
	mCapacity = other.mCapacity;

	other.mBuffer = nullptr;

	return *this;
}

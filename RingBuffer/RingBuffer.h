#pragma once

class RingBuffer final
{
public:
	RingBuffer();
	RingBuffer(const int capacity);
	RingBuffer(const RingBuffer& other) = delete;
	RingBuffer(RingBuffer&& other) = delete;
	~RingBuffer();

	RingBuffer& operator=(const RingBuffer& other) = delete;

public:
	bool TryEnqueue(const char* data, const int length);
	bool TryDequeue(char* outDest, const int length);
	bool TryPeek(char* outDest, const int length) const;
	void Reserve(const int newCapacity);
	bool IsEmpty();
	bool IsFull();
	void Clear();

	int GetCapacity() const;
	int GetSize() const;
	int GetUnusedSize() const;
	int GetDirectEnqueueableSize() const;
	int GetDirectDequeueableSize() const;

	char* GetFront() const;
	char* GetRear() const;
	void MoveFront(const int offset);
	void MoveRear(const int offset);

private:
	RingBuffer& operator=(RingBuffer&& other) noexcept;

private:
	enum { DEFAULT_SIZE = 2048 };

	char* mBuffer;
	int mFront;
	int mRear;
	int mCapacity;
};
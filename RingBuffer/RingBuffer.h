#pragma once

class RingBuffer final
{
public:
	RingBuffer();
	RingBuffer(int capacity);
	RingBuffer(const RingBuffer& other) = delete;
	RingBuffer(RingBuffer&& other) = delete;
	~RingBuffer();

	RingBuffer& operator=(const RingBuffer& other) = delete;

public:
	bool TryEnqueue(const char* data, int length);
	bool TryDequeue(char* outDest, int length);
	bool TryPeek(char* outDest, int length) const;
	void Reserve(int newCapacity);
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
	void MoveFront(int offset);
	void MoveRear(int offset);

private:
	RingBuffer& operator=(RingBuffer&& other) noexcept;

private:
	enum { DEFAULT_SIZE = 2048 };

	char* mBuffer;
	int mFront;
	int mRear;
	int mCapacity;
};
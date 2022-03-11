#pragma once

class AllocationHistoryArray final
{
public:
	AllocationHistoryArray(const size_t capacity = 4096);
	AllocationHistoryArray(AllocationHistoryArray& other) = delete;
	~AllocationHistoryArray();

	void operator=(AllocationHistoryArray& other) = delete;

	void Add(const AllocationHistory& history);
	bool Remove(const AllocationHistory& history);
	bool Contains(const AllocationHistory& history) const;
private:

	AllocationHistory* mHistories;
	size_t mCapacity;
	size_t mSize;
};
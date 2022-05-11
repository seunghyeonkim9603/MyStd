#include <assert.h>
#include <conio.h>
#include <string>
#include <stdlib.h>

#include "RingBuffer.h"

#define DEFAULT_SIZE (128)
#define TEST_STRING ("1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345")

int main(void)
{
	printf(TEST_STRING);
	_getch();

	const int ringBufferMinSize = 100;
	const int ringBufferMaxSize = 1000;

	srand(100);

	while (true)
	{
		int ringBufferSize = rand() % (ringBufferMaxSize - ringBufferMinSize + 1) + ringBufferMinSize;

		RingBuffer ringBuffer(ringBufferSize);
		std::string str = TEST_STRING;

		char peekBuffer[2048];
		char dequeueBuffer[2048];

		for (int i = 0; i < 1000000; ++i)
		{
			int unusedSize = ringBuffer.GetUnusedSize();
			int maxEnqueueSize = (int)str.length() < unusedSize ? (int)str.length() : unusedSize;

			int enqueueSize = (rand() % maxEnqueueSize) + 1;

			assert(ringBuffer.TryEnqueue(str.c_str(), enqueueSize) == true);
			assert(ringBuffer.GetCapacity() == ringBuffer.GetSize() + ringBuffer.GetUnusedSize());

			int dequeueSize = (rand() % ringBuffer.GetSize()) + 1;

			assert(ringBuffer.TryPeek(peekBuffer, dequeueSize) == true);
			assert(ringBuffer.TryDequeue(dequeueBuffer, dequeueSize) == true);
			assert(memcmp(peekBuffer, dequeueBuffer, dequeueSize) == 0);

			assert(ringBuffer.GetCapacity() == ringBuffer.GetSize() + ringBuffer.GetUnusedSize());

			dequeueBuffer[dequeueSize] = '\0';
			printf(dequeueBuffer);

			str = str.substr(enqueueSize, str.length() - enqueueSize);
			str.append(dequeueBuffer);
		}
		assert(ringBuffer.GetCapacity() == ringBufferSize);
		printf("\n");
	}
	return 0;
}
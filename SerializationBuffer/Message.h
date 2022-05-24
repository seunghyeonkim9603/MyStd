#pragma once

class Message final
{
public:
	Message();
	Message(int capacity);
	Message(const Message& other);
	Message(Message&& other) noexcept;
	~Message();

	void	Reserve(int capacity);
	void	Release();
	void	Clear();
	int		GetCapacity() const;
	int		GetSize() const;
	char*	GetBuffer();
	int		MoveWritePos(int offset);
	int		MoveReadPos(int offset);

	Message& operator=(const Message& other);
	Message& operator=(Message&& other) noexcept;

	Message& operator<<(unsigned char val);
	Message& operator<<(char val);
	Message& operator<<(bool val);
	Message& operator<<(short val);
	Message& operator<<(unsigned short val);
	Message& operator<<(int val);
	Message& operator<<(unsigned int val);
	Message& operator<<(long val);
	Message& operator<<(unsigned long val);
	Message& operator<<(long long val);
	Message& operator<<(unsigned long long val);
	Message& operator<<(float val);
	Message& operator<<(double val);

	Message& operator>>(unsigned char& val);
	Message& operator>>(char& val);
	Message& operator>>(bool& val);
	Message& operator>>(short& val);
	Message& operator>>(unsigned short& val);
	Message& operator>>(int& val);
	Message& operator>>(unsigned int& val);
	Message& operator>>(long& val);
	Message& operator>>(unsigned long& val);
	Message& operator>>(long long& val);
	Message& operator>>(unsigned long long& val);
	Message& operator>>(float& val);
	Message& operator>>(double& val);

	inline Message& Write(const char* str, int size);
	inline Message& Read(char* buffer, int size);
private:
	inline int getEnquableSize() const;

private:
	enum { DEFAULT_SIZE = 256 };
	int mCapacity;
	char* mBuffer;
	char* mFront;
	char* mRear;
};
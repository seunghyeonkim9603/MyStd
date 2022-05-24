#include <memory>

#include "Message.h"

Message::Message()
    : Message(DEFAULT_SIZE)
{
}

Message::Message(int capacity)
    : mCapacity(capacity),
    mBuffer(new char[capacity]),
    mFront(mBuffer),
    mRear(mBuffer)
{
}

Message::Message(const Message& other)
    : mCapacity(other.mCapacity),
    mBuffer(new char[other.mCapacity]),
    mFront(mBuffer + (other.mFront - other.mBuffer)),
    mRear(mBuffer + (other.mRear - other.mBuffer))
{
    memcpy(mBuffer, other.mBuffer, mCapacity);
}

Message::Message(Message&& other) noexcept
    : mCapacity(other.mCapacity),
    mBuffer(other.mBuffer),
    mFront(other.mFront),
    mRear(other.mRear)
{
    other.mBuffer = nullptr;
}

Message::~Message()
{
    delete mBuffer;
}

void Message::Reserve(int capacity)
{
    if (capacity <= mCapacity)
    {
        return;
    }
    Message temp(capacity);
    
    temp.Write(mFront, GetSize());

    *this = std::move(temp);
}

void Message::Release()
{
    delete mBuffer;
    mBuffer = nullptr;
}

void Message::Clear()
{
    mFront = mBuffer;
    mRear = mBuffer;
}

int Message::GetCapacity() const
{
    return mCapacity;
}

int Message::GetSize() const
{
    return static_cast<int>(mRear - mFront);
}

char* Message::GetBuffer()
{
    return mFront;
}

int Message::MoveWritePos(int offset)
{
    mRear += offset;
    return offset;
}

int Message::MoveReadPos(int offset)
{
    mFront += offset;
    return offset;
}

Message& Message::operator=(const Message& other)
{
    if (mBuffer != nullptr)
    {
        delete mBuffer;
    }
    mBuffer = new char[other.mCapacity];
    memcpy(mBuffer, other.mBuffer, other.mCapacity);
    mCapacity = other.mCapacity;
    mFront = mBuffer + (other.mFront - other.mBuffer);
    mRear = mBuffer + (other.mRear - other.mBuffer);

    return *this;
}

Message& Message::operator=(Message&& other) noexcept
{
    if (mBuffer != nullptr)
    {
        delete mBuffer;
    }
    mBuffer = other.mBuffer;
    mCapacity = other.mCapacity;
    mFront = other.mFront;
    mRear = other.mRear;

    delete other.mBuffer;
    other.mBuffer = nullptr;

    return *this;
}

Message& Message::operator<<(unsigned char val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    unsigned char* rear = reinterpret_cast<unsigned char*>(mFront);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(char val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    char* rear = reinterpret_cast<char*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(bool val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    bool* rear = reinterpret_cast<bool*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(short val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    short* rear = reinterpret_cast<short*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(unsigned short val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    unsigned short* rear = reinterpret_cast<unsigned short*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(int val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    int* rear = reinterpret_cast<int*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(unsigned int val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    unsigned int* rear = reinterpret_cast<unsigned int*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(long val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    long* rear = reinterpret_cast<long*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(unsigned long val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    unsigned long* rear = reinterpret_cast<unsigned long*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(long long val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    long long* rear = reinterpret_cast<long long*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(unsigned long long val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    unsigned long long* rear = reinterpret_cast<unsigned long long*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(float val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    float* rear = reinterpret_cast<float*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator<<(double val)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < sizeof(val))
    {
        Reserve(mCapacity * 2);
    }
    double* rear = reinterpret_cast<double*>(mRear);

    *rear = val;
    ++rear;
    mRear = reinterpret_cast<char*>(rear);

    return *this;
}

Message& Message::operator>>(unsigned char& outVal)
{
    outVal = *reinterpret_cast<unsigned char*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(char& outVal)
{
    outVal = *reinterpret_cast<char*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(bool& outVal)
{
    outVal = *reinterpret_cast<bool*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(short& outVal)
{
    outVal = *reinterpret_cast<short*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(unsigned short& outVal)
{
    outVal = *reinterpret_cast<unsigned short*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(int& outVal)
{
    outVal = *reinterpret_cast<int*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(unsigned int& outVal)
{
    outVal = *reinterpret_cast<unsigned int*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(long& outVal)
{
    outVal = *reinterpret_cast<long*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(unsigned long& outVal)
{
    outVal = *reinterpret_cast<unsigned long*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(long long& outVal)
{
    outVal = *reinterpret_cast<long long*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(unsigned long long& outVal)
{
    outVal = *reinterpret_cast<unsigned long long*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(float& outVal)
{
    outVal = *reinterpret_cast<float*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

Message& Message::operator>>(double& outVal)
{
    outVal = *reinterpret_cast<double*>(mFront);
    mFront += sizeof(outVal);

    return *this;
}

inline Message& Message::Write(const char* str, int size)
{
    int enquableSize = getEnquableSize();

    if (enquableSize < size)
    {
        Reserve(mCapacity * 2);
    }
    memcpy(mRear, str, size);
    mRear += size;

    return *this;
}

inline Message& Message::Read(char* outBuffer, int size)
{
    memcpy(outBuffer, mFront, size);
    mFront += size;

    return *this;
}

inline int Message::getEnquableSize() const
{
    return mBuffer + mCapacity - mRear;
}

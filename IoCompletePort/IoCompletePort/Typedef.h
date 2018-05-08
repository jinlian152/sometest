#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <assert.h>
#include <mutex>


#define floatEqualZero(x) (((x)>=-0.000001) && ((x)<=0.000001))
#define MIN(a, b) (a)<(b)?(a):(b)
#define MAX(a, b) (a)>(b)?(a):(b)

template<class Mutex>
class AutoLock
{
public:
	AutoLock(Mutex *mutex)
		:mutex_(mutex)
	{
		mutex_->lock();
	}

	~AutoLock()
	{
		mutex_->unlock();
		mutex_ = NULL;
	}

private:
	Mutex				*mutex_;
};


enum ENUM_OPCODE
{
	SMSG_A,
	CMSG_A,
	SMSG_B,
	CMSG_B,
};


#pragma pack(push, 1)

struct NetPacketHeader
{
	int							opcode_;
	int				size_;
}; 


struct SNetPacketA : public NetPacketHeader
{
	char buffer[16];
public:
	SNetPacketA()
	{
		opcode_ = SMSG_A;
		size_ = sizeof(*this);
	}
};

struct SNetPacketB : public NetPacketHeader
{
	char buffer[32];

public:
	SNetPacketB()
	{
		opcode_ = SMSG_B;
		size_ = sizeof(*this);
	}
};

struct CNetPacketA : public NetPacketHeader
{
	char buffer[16];

public:
	CNetPacketA()
	{
		opcode_ = CMSG_A;
		size_ = sizeof(*this);
	}
};

struct CNetPacketB : public NetPacketHeader
{
	char buffer[32];
public:
	CNetPacketB()
	{
		opcode_ = CMSG_B;
		size_ = sizeof(*this);
	}
};

#pragma pack(pop)



namespace
{
	typedef volatile LONG AtomicType;

	bool compareAndSwap(AtomicType *value, long newValue, long oldValue)
	{
		return (::InterlockedCompareExchange(value, newValue, oldValue) == oldValue);
	}
}
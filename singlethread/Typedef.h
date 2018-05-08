#ifndef _TYPE_DEF_H_
#define _TYPE_DEF_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>      
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <error.h>
#include <sys/epoll.h>
#include <time.h>

#define floatEqualZero(x) (((x)>=-0.000001) && ((x)<=0.000001))
#define MIN(a, b) ((a)<(b)?(a):(b))
#define MAX(a, b) ((a)>(b)?(a):(b))

unsigned long GetTickCount(); 

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
	int							size_;
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
	typedef volatile int AtomicType;

	bool compareAndSwap(AtomicType *value, long newValue, long oldValue)
	{
		return (::__sync_val_compare_and_swap (value, oldValue, newValue) == oldValue);
	}
}


#endif //#define _TYPE_DEF_H_
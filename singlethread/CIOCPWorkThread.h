#pragma once
#include "threadBase.h"

#define  DEF_MAX_SIZE_TO_WAIT	256

class CWorkThread : public ThreadBase
{
public:
	CWorkThread(int epollFd)
		: epollFd_(epollFd)
	{

	}
	~CWorkThread(void);
	virtual	int preRunKernel()
	{
		return 0;
	}

	virtual void Kernel();
private:
	int								epollFd_;
	struct epoll_event				epEvent[DEF_MAX_SIZE_TO_WAIT];
};


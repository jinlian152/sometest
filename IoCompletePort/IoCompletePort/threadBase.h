#ifndef DEF_ThreadBase_INCLUDE
#define DEF_ThreadBase_INCLUDE

#include "typedef.h"

enum enumThreadState
{
	enumThreadInit,
	enumThreadStart,
	enumThreadRun,
	enumThreadPending,
	enumThreadEnd,
};

class ThreadBase
{
public:
	ThreadBase(void);
	virtual ~ThreadBase(void);

	void stop();
	void start();
	void pause();
	void resume();
	bool isRuning();

	void setThreadState(enumThreadState state)
	{
		thradState_ = state;
	}
	
	enumThreadState getThreadState()
	{
		return thradState_;
	}

public:
	void Run();

public:
	virtual bool init()
	{
		return true;
	}

	virtual void clear()
	{

	}

	virtual	int preRunKernel()
	{
		return 0;
	}

	virtual void Kernel() = 0;

private:
	HANDLE														thread_;
	enumThreadState												thradState_;
};

#endif	//DEF_ThreadBase_INCLUDE
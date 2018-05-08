#pragma once
#include "typedef.h"

class Mutex
{
public:
	Mutex(void)
	{
		InitializeCriticalSection(&ctlSection_);
	}
	~Mutex(void)
	{
		Delete();
	}

	void lock()
	{
		EnterCriticalSection(&ctlSection_);
	}

	void unlock()
	{
		LeaveCriticalSection(&ctlSection_);
	}

	void tryLock()
	{
		TryEnterCriticalSection(&ctlSection_);
	}

	void Delete()
	{
		DeleteCriticalSection(&ctlSection_);
	}

private:
	CRITICAL_SECTION			ctlSection_;
};


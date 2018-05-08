#include "stdafx.h"
#include "threadBase.h"
#include <iostream>

DWORD __stdcall threadRun(void *arg)
{
	ThreadBase *thread = (ThreadBase*)arg;
	thread->Run();
	return 0;
}

ThreadBase::ThreadBase(void)
{
	thradState_ = enumThreadInit;
}


ThreadBase::~ThreadBase(void)
{
}

void ThreadBase::start()
{
	try
	{
		thread_ = CreateThread(NULL, 0, threadRun, this, 0, 0);
		setThreadState(enumThreadStart);
	}
	catch(...)
	{
		std::cout<<"Thread start failed!"<<std::endl;
	}
}

void ThreadBase::pause()
{
	thradState_ = enumThreadPending;
}

void ThreadBase::resume()
{
	thradState_ = enumThreadRun;
}

bool ThreadBase::isRuning()
{
	return thradState_ == enumThreadRun;
}

void ThreadBase::stop()
{
	clear();
	setThreadState(enumThreadEnd);
	WaitForSingleObject(thread_, INFINITE);
	CloseHandle(thread_);
}

void ThreadBase::Run()
{
	while (true)
	{
		if (enumThreadRun == thradState_)
			Kernel();
		else if (enumThreadStart == thradState_)
		{
			thradState_ = enumThreadRun;
			if (0 !=  preRunKernel())
			{
				printf("Preinit failed!, thread runing failed\n");
				break;
			}
			Kernel();
		}
		else if (enumThreadPending == thradState_)
		{
			Sleep(100);
			continue;
		}
		else if (enumThreadEnd == thradState_)
			break;
	}
}


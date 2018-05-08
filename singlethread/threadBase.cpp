#include "threadBase.h"
#include <iostream>

void*  threadRun(void *arg)
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
		 if (0 !=  pthread_create(&thread_, NULL, threadRun, this))
		 {
			 std::cout<<"Thread start failed!"<<std::endl;
		 }
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
	pthread_join(thread_, NULL);
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
			sleep(10);
			continue;
		}
		else if (enumThreadEnd == thradState_)
			break;
	}
}


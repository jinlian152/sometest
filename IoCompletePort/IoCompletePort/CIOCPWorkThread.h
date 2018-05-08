#pragma once
#include "threadBase.h"

class CIOCPWorkThread : public ThreadBase
{
public:
	CIOCPWorkThread(HANDLE handle)
		: hIOCP(handle)
	{

	}
	~CIOCPWorkThread(void);
	virtual	int preRunKernel()
	{
		return 0;
	}

	virtual void Kernel();
private:
	HANDLE				hIOCP;
};


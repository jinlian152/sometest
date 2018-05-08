// IoCompletePort.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CListener.h"
#include "CSessionMgr.h"
#include "CTcpConnectionMgr.h"
#include "CSessionMgr.h"


HANDLE hevent;

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)  
{  
	switch (dwCtrlType) 
	{ 
	case CTRL_C_EVENT: 
	case CTRL_CLOSE_EVENT: 
	case CTRL_BREAK_EVENT:
		SetEvent(hevent);
		return TRUE;
	default: 
		return FALSE; 
	}
} 

int _tmain(int argc, _TCHAR* argv[])
{
	hevent = CreateEvent(NULL, true, false, "iocp");

	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		printf("Only one iocp programme can be launched!\n");
		return -1;
	}

	CListener listener;
	listener.init();
	listener.start();

	sCSessionMgr.init();
	sCSessionMgr.start();


	SetConsoleCtrlHandler(HandlerRoutine, true);
	WaitForSingleObject(hevent, INFINITE);
	ResetEvent(hevent);

	listener.stop();
	sCSessionMgr.stop();
	return 0;
}


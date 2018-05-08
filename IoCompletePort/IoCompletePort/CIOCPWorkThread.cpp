#include "stdafx.h"
#include "CIOCPWorkThread.h"
#include "IOData.h"
#include "CTcpConnection.h"

CIOCPWorkThread::~CIOCPWorkThread(void)
{
}

void CIOCPWorkThread::Kernel()
{
	DWORD dataLen = 0;
	CTcpConnection *connection = NULL;
	LPOVERLAPPED overlaped;
	IOData *ioData = NULL;

	if (GetQueuedCompletionStatus(hIOCP, &dataLen, (PULONG_PTR)&connection, &overlaped, 300))
	{
		if (NULL != connection)
		{
			ioData = (IOData *)overlaped;
			switch (ioData->type_)
			{
			case IO_TYPE_READ:
				{
					if (dataLen > 0)
						connection->startNextRead(dataLen);
					else
					{
						//char buffer[4096] = { 0 };
						//int len = recv(connection->getSocket(), buffer, 4096, 0);
						connection->close();
					}
				}
				break;
			case IO_TYPE_WRITE:
				{
					connection->startNextWrite(dataLen);
				}
				break;
			default:
				break;
			}
		}
	}
	else
	{
		DWORD err = WSAGetLastError();
		if(err == ERROR_NETNAME_DELETED && NULL != connection)
			connection->close();
	}
	
}

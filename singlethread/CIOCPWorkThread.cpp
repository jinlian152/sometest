#include "Typedef.h"
#include "CIOCPWorkThread.h"
#include "CTcpConnection.h"
#include "CSessionMgr.h"

CWorkThread::~CWorkThread(void)
{
}

void CWorkThread::Kernel()
{
	int num = epoll_wait(epollFd_, epEvent, DEF_MAX_SIZE_TO_WAIT, 500);
	if (num > 0)
	{
		for (int i = 0; i != num; ++i)
		{
			printf("WorkThread Enter!!!!!!\n");
			unsigned int s = GetTickCount();
			bool isProcessOk = true;
			const epoll_event &ent = epEvent[i];
			CTcpConnection *connection = (CTcpConnection *)epEvent[i].data.ptr;
			if (ent.events & EPOLLERR || ent.events & EPOLLRDHUP || ent.events & EPOLLHUP)
			{
				connection->Close();
				continue;
			}
			if (ent.events & EPOLLIN)
			{
				isProcessOk &= connection->Read();
			}

			if (ent.events & EPOLLOUT)
			{
				isProcessOk &= connection->Write();
			}

			if (!isProcessOk)
			{
				connection->Close();
				continue;
			}

			connection->ModifyRegistInfo();

			unsigned int e = GetTickCount();
			printf("WorkTHREAD Time:%d\n", e - s);
		}
	}

	if (num < 0)
	{
		int err = errno;
		printf("Epoll_WAIT erron:%d\n", err);
	}

	sCSessionMgr.update();
}

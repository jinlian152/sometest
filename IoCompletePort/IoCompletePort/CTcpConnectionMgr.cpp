#include "stdafx.h"
#include "CTcpConnectionMgr.h"
#include "CIOCPWorkThread.h"


class PushSendThread : public ThreadBase
{
public:
	PushSendThread(CTcpConnectionMgr* manager)
		: cntMgr_(manager)
	{

	}
	~PushSendThread()
	{
		cntMgr_ = NULL;
	}

	virtual void Kernel()
	{
		cntMgr_->mutex_.lock();
		if (cntMgr_->connections_.empty())
			Sleep(10);
		else
		{
			std::map<int, CTcpConnection*>::iterator iter = cntMgr_->connections_.begin();
			for ( ; iter != cntMgr_->connections_.end(); )
			{
				if (iter->second->doPushSend())
					iter++;
				else
					iter = cntMgr_->connections_.begin();//TODO:
			}
		}
		cntMgr_->mutex_.unlock();
		Sleep(1);
	}

private:
	CTcpConnectionMgr				*cntMgr_;
};




CTcpConnectionMgr* CTcpConnectionMgr::connectionMgr = new CTcpConnectionMgr();

CTcpConnectionMgr::CTcpConnectionMgr(void)
	: iocpIndex_(0)
{
	threadVec_.clear();
}


CTcpConnectionMgr::~CTcpConnectionMgr(void)
{
	ThreadVecIter iter = threadVec_.begin();
	for ( ; iter != threadVec_.end(); )
	{
		if (NULL != *iter)
		{
			(*iter)->stop();
			delete *iter;
			*iter = NULL;
		}
		threadVec_.erase(iter);
	}
}

void CTcpConnectionMgr::init()
{
	threadVec_.push_back(new PushSendThread(this));

	SYSTEM_INFO sysInfo;
	memset(&sysInfo, 0, sizeof(SYSTEM_INFO));
	GetSystemInfo(&sysInfo);
	int num = sysInfo.dwNumberOfProcessors * 2;
	for (int i = 0; i != num;  ++i)
	{
		HANDLE cp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
		if (INVALID_HANDLE_VALUE != cp)
		{
			iocpVec_.push_back(cp);
			threadVec_.push_back(new CIOCPWorkThread(cp));
		}
	}
}

void CTcpConnectionMgr::startWorkThread()
{
	ThreadVecIter iter = threadVec_.begin();
	for ( ; iter != threadVec_.end(); ++iter)
	{
		(*iter)->init();
		(*iter)->start();
	}
}

void CTcpConnectionMgr::addConnection(CTcpConnection *connection)
{
	AutoLock<Mutex> lock(&mutex_);
	connections_.insert(std::make_pair(connection->getSocket(), connection));

	int index = iocpIndex_++ % iocpVec_.size();
	connection->setIOCP(iocpVec_[index]);
	connection->setMgr(this);
	CreateIoCompletionPort((HANDLE)connection->getSocket(), connection->getIOCP(), (ULONG_PTR)connection, 0);
	connection->startNextRead(0);
}

void CTcpConnectionMgr::removeConnection(CTcpConnection *connection)
{
	AutoLock<Mutex> lock(&mutex_);
	ConnectionMapIter iter = connections_.find(connection->getSocket());
	if (iter != connections_.end())
		connections_.erase(iter);
}

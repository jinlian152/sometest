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
		unsigned int s = GetTickCount();
		cntMgr_->mutex_.lock();
		if (!cntMgr_->connections_.empty())
		{
			std::map<int, CTcpConnection*>::iterator iter = cntMgr_->connections_.begin();
			for ( ; iter != cntMgr_->connections_.end(); )
			{
				if (iter->second->doPushSend())
					iter++;
				else
					iter = cntMgr_->connections_.begin();//TODO:whether use vector instead of map to avoid unnecessary and verbose circle process?
			}
		}
		cntMgr_->mutex_.unlock();
		unsigned int e = GetTickCount();
		printf("PushSEND Time:%d\n", e - s);
		sleep(1);
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
	//threadVec_.push_back(new PushSendThread(this));

	int cpuNum = sysconf(_SC_NPROCESSORS_ONLN);
	int num = cpuNum * 2;
	for (int i = 0; i != num;  ++i)
	{
		int epollfd = epoll_create(1024);
		if (-1 != epollfd)
		{
			epollFdVec_.push_back(epollfd);
			threadVec_.push_back(new CWorkThread(epollfd));
		}
		else
		{
			assert(false);
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

	int index = iocpIndex_++ % epollFdVec_.size();
	connection->setEpollFd(epollFdVec_[index]);//add before regist
	connection->setMgr(this);
	connection->Regist();
}

void CTcpConnectionMgr::removeConnection(CTcpConnection *connection)
{
	AutoLock<Mutex> lock(&mutex_);
	connection->UnRegist();
	ConnectionMapIter iter = connections_.find(connection->getSocket());
	if (iter != connections_.end())
		connections_.erase(iter);
}

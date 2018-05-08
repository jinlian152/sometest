#pragma once
#include "CTcpConnection.h"
#include <vector>
#include "threadBase.h"
#include "Mutex.h"
#include <map>

class CTcpConnectionMgr
{
	typedef std::vector<int>							EpollFdVec;
	typedef std::vector<int>::iterator				EpollFdVecIter;

	typedef std::vector<ThreadBase*>					ThreadVec;
	typedef std::vector<ThreadBase*>::iterator			ThreadVecIter;

	typedef std::map<int, CTcpConnection*>				ConnectionMap;
	typedef std::map<int, CTcpConnection*>::iterator	ConnectionMapIter;

public:

	friend class PushSendThread;
	CTcpConnectionMgr(void);
	~CTcpConnectionMgr(void);

	void init();
	void startWorkThread();
	void addConnection(CTcpConnection *connection);
	void removeConnection(CTcpConnection *connection);
	
public:
	static CTcpConnectionMgr* instance()
	{
		return connectionMgr;
	}

private:
	EpollFdVec					epollFdVec_;
	ThreadVec					threadVec_;
	ConnectionMap				connections_;
	Mutex						mutex_;
	int							iocpIndex_;

private:
	static CTcpConnectionMgr					*connectionMgr;
};

#define sTcpConnectionMgr			(*CTcpConnectionMgr::instance())

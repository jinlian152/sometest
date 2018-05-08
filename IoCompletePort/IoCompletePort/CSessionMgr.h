#pragma once
#include <list>
#include "Mutex.h"
//#include "Session.h"
#include "CSyncQueue.h"
#include "threadBase.h"

class Session;

class CSessionMgr : public ThreadBase
{
	typedef std::list<Session*>						SessionList;
	typedef std::list<Session*>::iterator			SessionListIter;

public:
	CSessionMgr(void);
	~CSessionMgr(void);

	void addSession(Session *ses);
	void removeSession(Session *ses);

	void checkSession();
	void update();

	static CSessionMgr* instance()
	{
		return sesMgr_;
	}

public:
	virtual void Kernel();

private:
	SessionList							sessionList_;
	Mutex								mutex_;
	CSyncQueue<Session*>				watingSesion_;
private:
	static CSessionMgr					*sesMgr_;
};

#define sCSessionMgr			(*CSessionMgr::instance())
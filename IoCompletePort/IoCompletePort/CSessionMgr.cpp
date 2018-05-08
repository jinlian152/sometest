#include "stdafx.h"
#include "CSessionMgr.h"
#include "Session.h"
#include "CTcpConnection.h"

CSessionMgr* CSessionMgr::sesMgr_ = new CSessionMgr();

CSessionMgr::CSessionMgr(void)
{
}


CSessionMgr::~CSessionMgr(void)
{
}

void CSessionMgr::addSession(Session *ses)
{
	watingSesion_.enqueue(ses);
}

void CSessionMgr::checkSession()
{
	Session *ses = NULL;
	while (watingSesion_.deEnqueue(ses) == 0)
		sessionList_.push_back(ses);

	SessionListIter iter = sessionList_.begin();
	for ( ; iter != sessionList_.end(); )
	{
		if (!(*iter)->isActive())
		{
			Session *ses = *iter;
			iter = sessionList_.erase(iter);

			ses->destroy();
			delete ses;
			ses = NULL;
		}
		else
			++iter;
	}
}

void CSessionMgr::update()
{
	checkSession();

	SessionListIter iter = sessionList_.begin();
	for ( ; iter != sessionList_.end(); ++iter)
	{
		if ((*iter)->isActive())
			(*iter)->update();
	}
	//TODO:need to prevent this thread runing empty.
	if (sessionList_.empty())
		Sleep(1);
}

void CSessionMgr::Kernel()
{
	update();
}

void CSessionMgr::removeSession(Session *ses)
{
	SessionListIter iter = sessionList_.begin();
	for ( ; iter != sessionList_.end(); ++iter)
	{
		if (*iter == ses)
		{
			sessionList_.erase(iter);
			break;
		}
	}
}

#include "CListener.h"
#include "CSessionMgr.h"
#include "CTcpConnectionMgr.h"
#include "CSessionMgr.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>

int main()
{
	CListener listener;
	listener.init();
	listener.start();

	sCSessionMgr.init();
	//sCSessionMgr.start();

	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGQUIT);
	sigaddset(&sset, SIGTERM);
	sigprocmask(SIG_BLOCK, &sset, NULL);
	int sig;
	sigwait(&sset, &sig);
	return 0;
}
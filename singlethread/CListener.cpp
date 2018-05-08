#include "Typedef.h"
#include "CListener.h"
#include "CTcpConnectionMgr.h"
#include "CSessionMgr.h"
#include "Session.h"
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h> 

CListener::CListener(void)
	:listenSocket_(0)
{
}


CListener::~CListener(void)
{
}

int CListener::onInitAddr()
{
	memset(&addr_, 0, sizeof(struct sockaddr_in));
	addr_.sin_family = AF_INET;
	addr_.sin_addr.s_addr = htonl(INADDR_ANY);

	m_listenPort = 7001;
	addr_.sin_port = htons(m_listenPort);
	printf("Init addr complete!\n");
	return 0;
}

bool CListener::init()
{
	sTcpConnectionMgr.init();
	sTcpConnectionMgr.startWorkThread();
	return true;
}

int CListener::preRunKernel()
{
	if (listenSocket_ > 0)
		return 0;
	if ( (listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		printf("Create listen socket failed!\n");
		return -1;
	}

	u_long flags = ::fcntl(listenSocket_, F_GETFL,0);
	::fcntl(listenSocket_, F_SETFL, flags | O_NONBLOCK);

	int option = 1;
	setsockopt(listenSocket_, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, 4);

	onInitAddr();

	if(bind(listenSocket_, (struct sockaddr *)&addr_, sizeof(sockaddr_in)) < 0)
		return -1;

	if (listen(listenSocket_, SOMAXCONN) < 0)
		return -1;
	printf("Listen complete!\n");
	return 0;
}

void CListener::Kernel()
{
	sockaddr_in clientAddr; 
	fd_set readSet, errSet;
	FD_ZERO(&readSet);
	FD_ZERO(&errSet);
	FD_SET(listenSocket_, &readSet);
	FD_SET(listenSocket_, &errSet);

	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	int n = select(listenSocket_ + 1, &readSet, NULL, &errSet, &tv);
	if (n < 0 || FD_ISSET(listenSocket_, &errSet))
	{
		printf("linsten thread kernel error, error No. :%d \n", errno);
		setThreadState(enumThreadEnd);
		return;
	}
	if (n > 0 && FD_ISSET(listenSocket_, &readSet))
	{
		int socket;
		int len =  sizeof(sockaddr_in);
		if (-1 == (socket = accept(listenSocket_, (sockaddr*)&clientAddr, (socklen_t*)&len)))
		{
			printf("linsten thread accept error, error No. :%d \n", errno);
			return;
		}

		onAccept(socket, inet_ntoa(clientAddr.sin_addr));
	}
}

void CListener::onAccept(int sock, std::string ip)
{
	//int size = 0;
	//setsockopt(sock, IPPROTO_TCP, SO_SNDBUF, (char *)&size, sizeof(int));

	unsigned int flags = ::fcntl(sock, F_GETFL,0);
	::fcntl(sock, F_SETFL, flags | O_NONBLOCK);

	unsigned int arg = 1;
	if (0 != setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&arg, sizeof(arg)))
		printf("Option ERROR!:%d\n", errno);

	CTcpConnection *conection = new CTcpConnection(sock);
	conection->setIp(ip);
	printf("Enter:%s \n", ip.c_str());

	Session *session = new Session();
	session->setConnection(conection);
	conection->setSession(session);
	session->setMgr(&sCSessionMgr);

	sCSessionMgr.addSession(session);
	sTcpConnectionMgr.addConnection(conection);
}

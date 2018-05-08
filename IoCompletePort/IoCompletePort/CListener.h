#pragma once
#include "threadBase.h"
#include <string>
#include <vector>
#include "CTcpConnectionMgr.h"

class CListener : public ThreadBase
{
public:
	CListener(void);
	~CListener(void);
	int onInitAddr();
	virtual bool init();
	virtual int preRunKernel();
	virtual void Kernel();
	void onAccept(SOCKET sock, std::string ip);

private:
	SOCKET						listenSocket_;
	unsigned short				m_listenPort;
	struct sockaddr_in			addr_;
};


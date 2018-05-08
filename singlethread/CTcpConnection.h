#pragma once
#include "Typedef.h"
#include "circleBuffer.h"
#include "CircularBuffer.h"
#include <string>
class Session;
class CTcpConnectionMgr;

class CTcpConnection
{
public:
	CTcpConnection(int socket)
		: socket_(socket)
		, session_(NULL)
		, writeLock_(0)
		, readLock_(0)
		, pushSend_(false)
		, readBuffer_(4096)
	{
		memset(temporyReadBuffer_, 0, 4096);
	}

	~CTcpConnection(void);

	void setIp(const std::string &ip);
	std::string getIp();
	void setEpollFd(int epollFd);
	void setMgr(CTcpConnectionMgr *mgr);
	int getEpollFd();
	void setSession(Session *ses);
	Session* getSession();
	int getSocket();

public:
	bool Write();
	bool Read();
	void onRead();
	NetPacketHeader* getPacketFromReadBuffer();
	void pushSend();
	bool doPushSend();
	void Close();

	bool Regist();
	bool UnRegist();
	bool ModifyRegistInfo(bool checkWrite = false);

public:
	bool TryAcquireWriteLock() { return compareAndSwap(&writeLock_, 1, 0);};
	bool ReleaseWriteLock() { return compareAndSwap(&writeLock_, 0, 1);};

	bool TryAcquireReadLock() { return compareAndSwap(&readLock_, 1, 0);};
	bool ReleaseReadLock() { return compareAndSwap(&readLock_, 0, 1);};
	
private:
	CTcpConnection(void);
	CTcpConnection& operator = (CTcpConnection&);

private:
	unsigned short					port_;
	std::string						ip_;
	int						socket_;
	Session							*session_;
	int								epollFd_;
	CTcpConnectionMgr				*mgr_;
	char							temporyReadBuffer_[4096];
	//CircleBuffer					readBuffer_;
	CircularBuffer					readBuffer_;
	struct epoll_event				epollEvent_;


	AtomicType						writeLock_;
	AtomicType						readLock_;
	volatile bool					pushSend_;
};

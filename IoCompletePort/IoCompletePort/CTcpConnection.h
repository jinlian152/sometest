#pragma once
#include "Typedef.h"
#include "IOData.h"
#include "circleBuffer.h"
#include "CircularBuffer.h"
class Session;
class CTcpConnectionMgr;

class CTcpConnection
{
public:
	CTcpConnection(SOCKET socket)
		: socket_(socket)
		, session_(NULL)
		, readIoData_(IO_TYPE_READ)
		, writeIoData_(IO_TYPE_WRITE)
		, writeLock_(0)
		, pushSend_(false)
		,readBuffer_(4096)
	{
		memset(temporyReadBuffer_, 0, 2048);
	}

	~CTcpConnection(void);

	void setIp(const std::string &ip);
	std::string getIp();
	void setIOCP(HANDLE iocp);
	void setMgr(CTcpConnectionMgr *mgr);
	HANDLE getIOCP();
	void setSession(Session *ses);
	Session* getSession();
	SOCKET getSocket();

public:
	bool startNextWrite(int lastWriteLen);
	bool startNextRead(int lastReadLen);
	void onRead();
	NetPacketHeader* getPacketFromReadBuffer();
	void pushSend();
	bool doPushSend();
	void close();

public:
	bool TryAcquireWriteLock() { return compareAndSwap(&writeLock_, 1, 0);};
	bool ReleaseWriteLock() { return compareAndSwap(&writeLock_, 0, 1);};
	
private:
	CTcpConnection(void);
	CTcpConnection& operator = (CTcpConnection&);

private:
	unsigned short					port_;
	std::string						ip_;
	SOCKET							socket_;
	Session							*session_;
	HANDLE							iocp_;
	CTcpConnectionMgr				*mgr_;
	char							temporyReadBuffer_[4096];
	//CircleBuffer					readBuffer_;
	CircularBuffer					readBuffer_;

	IOData							readIoData_;
	IOData							writeIoData_;

	volatile bool					pushSend_;
	AtomicType						writeLock_;


	char							buf[11];
};
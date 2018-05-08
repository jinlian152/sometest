#pragma once
#include "CSessionMgr.h"
#include "Mutex.h"
#include <list>
#include "CSyncBuffer.h"

enum ENUM_STATE
{
	enum_disable,
	enum_active,
	//enum_needInit,
};

class CTcpConnection;
class CSessionMgr;
struct NetPacketHeader;

class Session
{
	typedef void (Session::*HandlerType)(NetPacketHeader *pck);
	typedef std::list<NetPacketHeader*>						packetQueue;
	typedef std::list<NetPacketHeader*>::iterator			packetQueueIter;

public:
	Session(void);
	~Session(void);

	void setConnection(CTcpConnection *connection);
	CTcpConnection* getConnection();

	void destroy();
	void setMgr(CSessionMgr *mgr);
	void update();
	bool isActive();
	void kill();
	void disable();

	void pushPacket(NetPacketHeader *packet);
	void clearSendBuffer();
	void pushSendPacket(NetPacketHeader *packet);

	char* getSendData();
	int getRemainSendDataSize();
	void removeSendData(int len);

	void ProcessCAMsg(NetPacketHeader *header);
	void ProcessCBMsg(NetPacketHeader *header);

private:
	CTcpConnection					*connection_;
	CSessionMgr						*sessionMgr_;
	ENUM_STATE						state_;
	HandlerType						handler_[100];
	packetQueue						packetList_;
	Mutex							mutex_;
	CSyncBuffer						sendBuffer_;
};


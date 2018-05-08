#include "Session.h"
#include "CTcpConnection.h"

Session::Session(void)
	: state_(enum_active)
	, sendBuffer_()
{
	handler_[CMSG_A] = &Session::ProcessCAMsg;
	handler_[CMSG_B] = &Session::ProcessCBMsg;
}


Session::~Session(void)
{
}

void Session::destroy()
{
	if (NULL != connection_)
	{
		printf("Disconnect:%s\n", connection_->getIp().c_str());
		delete connection_;
		connection_ = NULL;
	}
}

void Session::setConnection(CTcpConnection *connection)
{
	connection_ = connection;
}

CTcpConnection* Session::getConnection()
{
	return connection_;
}

void Session::setMgr(CSessionMgr *mgr)
{
	sessionMgr_ = mgr;
}

bool Session::isActive()
{
	return state_ == enum_active;
}

void Session::kill()
{
	if (isActive())
		disable();
}

void Session::disable()
{
	state_ = enum_disable;
}

void Session::pushPacket(NetPacketHeader *packet)
{
	AutoLock<Mutex> lock(&mutex_);
	packetList_.push_back(packet);
}

char* Session::getSendData()
{
	return sendBuffer_.getRead();
}

int Session::getRemainSendDataSize()
{
	return sendBuffer_.getUsedSize();
}

void Session::removeSendData(int len)
{
	sendBuffer_.remove(len);
	sendBuffer_.restoration();
}

void Session::clearSendBuffer()
{
	AutoLock<Mutex> lock(&mutex_);
}

void Session::pushSendPacket(NetPacketHeader *packet)
{
	printf("Recv111111111111111111111111111111111111111111111111\n");
	if (packet->size_ != sendBuffer_.write((char*)packet, packet->size_))
		assert(false);
	connection_->Write();
}

void Session::update()
{
	printf("Recv22222222222222222222222222222222222222222222222\n");
	unsigned int s = GetTickCount();
	AutoLock<Mutex> lock(&mutex_);
	printf("3333333333333333333333333333333333333333\n");
	while (isActive() && !packetList_.empty())
	{
		printf("4444444444444444444444444444444444444444\n");
		packetQueueIter iter = packetList_.begin();
		(this->*handler_[(*iter)->opcode_])(*iter);
		delete [] (char *)*iter;
		*iter = NULL;
		packetList_.pop_front();
		unsigned int e = GetTickCount();
		printf("Session Process  Time:%d\n", e - s);
	}
	printf("55555555555555555555555555555555555555555\n");
}


///-------------------------------------------msg demo---------------------------------------------


void Session::ProcessCAMsg(NetPacketHeader *header)
{
	CNetPacketA *pA = (CNetPacketA *)header;
	printf("Recv: %s \n", pA->buffer);

	char *a = "I am an a!";
	SNetPacketA pSA;
	memcpy(pSA.buffer, a, strlen(a)+1);
	pushSendPacket(&pSA);
}

void Session::ProcessCBMsg(NetPacketHeader *header)
{
	CNetPacketB *pB = (CNetPacketB *)header;
	printf("Recv: %s \n", pB->buffer);

	char *b = "I am a b!";
	SNetPacketB pSB;
	memcpy(pSB.buffer, b, strlen(b)+1);
	pushSendPacket(&pSB);
}
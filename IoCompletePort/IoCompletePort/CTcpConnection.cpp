#include "stdafx.h"
#include "CTcpConnection.h"
#include "Session.h"
#include "CTcpConnectionMgr.h"

CTcpConnection::~CTcpConnection(void)
{
}

void CTcpConnection::setIp(const std::string &ip)
{
	ip_ = ip;
}
std::string CTcpConnection::getIp()
{
	return ip_;
}

void CTcpConnection::setIOCP(HANDLE iocp)
{
	iocp_ = iocp;
}

HANDLE CTcpConnection::getIOCP()
{
	return iocp_;
}

void CTcpConnection::setMgr(CTcpConnectionMgr *mgr)
{
	mgr_ = mgr;
}

void CTcpConnection::setSession(Session *ses)
{
	session_ = ses;
}

Session* CTcpConnection::getSession()
{
	return session_;
}

SOCKET CTcpConnection::getSocket()
{
	return socket_;
}

bool CTcpConnection::startNextWrite(int lastWriteLen)
{
	if (0 != lastWriteLen)
		session_->removeSendData(lastWriteLen);

	if (0 == session_->getRemainSendDataSize())
	{
		ReleaseWriteLock();
		return true;
	}
	 
	DWORD flags = 0;
	DWORD length = 0;
	WSABUF wbuf;
	wbuf.buf = session_->getSendData();
	wbuf.len = session_->getRemainSendDataSize();

	if (WSASend(socket_, &wbuf, 1, &length, flags, &writeIoData_, 0) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			close();
			return false;
		}
	}
	return true;
}

bool CTcpConnection::startNextRead(int lastReadLen)
{
	if (0 != lastReadLen && session_->isActive())
	{
		readBuffer_.Write(temporyReadBuffer_, lastReadLen);
		onRead();
	}
	int len = MIN(sizeof(temporyReadBuffer_), readBuffer_.GetFreeSpaceSize());

	if (0 == len)
		int a = 4;//debug
	//TODO: process the case of the "len" is zero.
	DWORD flags = 0;
	DWORD length = 0;
	WSABUF wbuf;
	wbuf.buf = temporyReadBuffer_;
	wbuf.len = len;

	//wbuf.buf = buf;
	//wbuf.len = 0;
	if (WSARecv(socket_, &wbuf, 1, &length, &flags, &readIoData_, 0) == SOCKET_ERROR)
	{
		DWORD err;
		if ((err = WSAGetLastError()) != WSA_IO_PENDING)
		{
			close();
			return false;
		}
	}
	return true;
}

void CTcpConnection::onRead()
{
	if (session_ && session_->isActive())
	{
		while (true)
		{
			NetPacketHeader	*packet = getPacketFromReadBuffer();
			if (NULL != packet)
				session_->pushPacket(packet);
			else
				break;
		}
	}
}

NetPacketHeader* CTcpConnection::getPacketFromReadBuffer()
{
	NetPacketHeader	packet;
	if (readBuffer_.GetUsedSpaceSize() < sizeof(packet))
		return NULL;

	readBuffer_.Read((char*)&packet, sizeof(packet));
	if (readBuffer_.GetUsedSpaceSize() < packet.size_)
		return NULL;

	char *buffer = new char[packet.size_];
	readBuffer_.Read(buffer, packet.size_);
	readBuffer_.Remove(packet.size_);
	return (NetPacketHeader*)buffer;
}

void CTcpConnection::close()
{
	closesocket(socket_);

	if (NULL != mgr_)
		mgr_->removeConnection(this);

	if (session_ && session_->isActive())
		session_->kill();
}

void CTcpConnection::pushSend()
{
	pushSend_ = true;
}

bool CTcpConnection::doPushSend()
{
	if (pushSend_)
	{
		pushSend_ = false;
		if (TryAcquireWriteLock())
			return startNextWrite(0);
	}
	return true;
}

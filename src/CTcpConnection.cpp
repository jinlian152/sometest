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

void CTcpConnection::setEpollFd(int epollFd)
{
	epollFd_ = epollFd;
}

int CTcpConnection::getEpollFd()
{
	return epollFd_;
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

int CTcpConnection::getSocket()
{
	return socket_;
}

bool CTcpConnection::Write()
{
	unsigned int s = GetTickCount();
	while (true)
	{
		int size = session_->getRemainSendDataSize();
		if (size == 0)
		{
			printf("Write size == 0000000000000000000!!!!!\n");
			break;
		}
		char *buf = session_->getSendData();
		int len = send(socket_, buf, size, 0);

		if (len <= 0)
		{
			int err = errno;
			if (err == EWOULDBLOCK || err == EAGAIN)
			{
				ReleaseWriteLock();
				printf("Write EAGAINNNNNNNNNNNNNNNNNNNNN!!!!!\n");
				break;
			}
			printf("Write ERRORRRRRRRRRRRRRRRRRRRR!!!!!\n");
			return false;
		}
		else if (len < size)
		{
			session_->removeSendData(len);
			printf("Write:len < size\n");
			break;
		}
		session_->removeSendData(len);
	}
	ReleaseWriteLock();
	printf("WriteWWWwWWWWWWWWWWWWWWWWWWWWW\n");

	unsigned int e = GetTickCount();
	//printf("Connection WRITE Time:%d\n", e - s);
	return true;
}

bool CTcpConnection::Read()
{
	if (TryAcquireReadLock())
	{
		while (true)
		{
			int size = MIN(sizeof(temporyReadBuffer_), readBuffer_.GetFreeSpaceSize());
			if (size > 0)
			{
				int len = recv(socket_, temporyReadBuffer_, size, 0);

				if (len <=  0)
				{
					int err = errno;
					if (err == EWOULDBLOCK || err == EAGAIN)
					{
						ReleaseReadLock();
						printf("Read EAGAIN!!!!!\n");
						break;
					}
					ReleaseReadLock();
					return false;
				}
				else if(session_->isActive())
				{
					readBuffer_.Write(temporyReadBuffer_, len);
					onRead();
				}

				if (len < size)
				{
					printf("Read:len < size\n");
					break;
				}
			}
		}
	}
	ReleaseReadLock();
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

void CTcpConnection::Close()
{
	close(socket_);

	if (NULL != mgr_)
		mgr_->removeConnection(this);

	if (session_ && session_->isActive())
		session_->kill();
}

bool CTcpConnection::Regist()
{
	epollEvent_.data.ptr = this;
	epollEvent_.events = EPOLLIN | EPOLLRDHUP | EPOLLET | EPOLLONESHOT;
	if (0 == epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, getSocket(), &epollEvent_))
	{
		printf("Connection registed :%s\n", getIp().c_str());
		return true; 
	}
	return false;
}

bool CTcpConnection::UnRegist()
{
	if (0 == epoll_ctl(getEpollFd(), EPOLL_CTL_DEL, getSocket(), &epollEvent_))
	{
		printf("Connection unregisted :%s\n", getIp().c_str());
		return true;
	}

	return false;
}

bool CTcpConnection::ModifyRegistInfo(bool checkWrite)
{
	unsigned int events = EPOLLRDHUP | EPOLLONESHOT;
	if (checkWrite && session_->getRemainSendDataSize() > 0)
	{
		TryAcquireWriteLock();			//different from iocp, there is not deliever request,just change the state of epoll to inspect the write event.
		events |= EPOLLOUT;
		printf("checkWrite-------------------------------------------\n");
	}

	epollEvent_.data.ptr = this;
	events |= EPOLLIN | EPOLLET;
	epollEvent_.events = events;
	if (0 == epoll_ctl(getEpollFd(), EPOLL_CTL_MOD, getSocket(), &epollEvent_))
	{
		return true;
	}
	printf("Rearm false!\n");
	return false;
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
		{
			printf("Rearm Success!!!\n");
			return ModifyRegistInfo(true);
		}
		else
			printf("Rearm failed!!!\n");
	}
	return true;
}

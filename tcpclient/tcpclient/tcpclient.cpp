// tcpclient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#pragma  comment(lib,"ws2_32.lib")

enum ENUM_OPCODE
{
	SMSG_A,
	CMSG_A,
	SMSG_B,
	CMSG_B,
};

#pragma pack(push, 1)

struct NetPacketHeader
{
	int				opcode_;
	int				size_;
}; 

struct SNetPacketA : public NetPacketHeader
{
	char buffer[16];
public:
	SNetPacketA()
	{
		opcode_ = SMSG_A;
		size_ = sizeof(*this);
	}
};

struct SNetPacketB : public NetPacketHeader
{
	char buffer[32];

public:
	SNetPacketB()
	{
		opcode_ = SMSG_B;
		size_ = sizeof(*this);
	}
};

struct CNetPacketA : public NetPacketHeader
{
	char buffer[16];

public:
	CNetPacketA()
	{
		opcode_ = CMSG_A;
		size_ = sizeof(*this);
	}
};

struct CNetPacketB : public NetPacketHeader
{
	char buffer[32];
public:
	CNetPacketB()
	{
		opcode_ = CMSG_B;
		size_ = sizeof(*this);
	}
};

#pragma pack(pop)

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;

	if( WSAStartup(MAKEWORD(2,2) , &wsaData )!=0)//0表示初始化成功
	{
		return 0;
	}

	SOCKET sockClient = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("192.168.154.129");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(7001);
	int n = connect(sockClient,(SOCKADDR *)&addrSrv,sizeof(SOCKADDR));

	if (n < 0)
	{
		DWORD err = WSAGetLastError();
		int a = 4;
	}
	//char recvBuf[100];
	
	while (true)
	{
		DWORD start = ::GetTickCount();
		char *a = "I am an a!";
		char *b = "I am a b!";

		CNetPacketA pA;
		memcpy(pA.buffer, a, strlen(a)+1);
		if (SOCKET_ERROR == send(sockClient,(char*)&pA, sizeof(pA), 0))
		{
			DWORD err = WSAGetLastError();
			int a = 4;
		}

		CNetPacketB pB;
		memcpy(pB.buffer, b, strlen(b)+1);

		if (SOCKET_ERROR == send(sockClient,(char *)&pB, sizeof(pB), 0))
		{
			DWORD err = WSAGetLastError();
			int a = 4;
		}


		char recvbuffer[1024] = { 0 };
		memset(recvbuffer, 0, 1024);
		int n = recv(sockClient, recvbuffer, 1024, 0);
		DWORD stop = ::GetTickCount();

		printf("Interval:%d\n", (stop - start) / 1000);
		if (0 == n)
			break;
		printf("%s\n", recvbuffer);
	}

	closesocket(sockClient);
	WSACleanup();
	return 0;
}


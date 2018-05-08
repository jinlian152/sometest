#include "stdafx.h"
#include "Typedef.h"

struct WSA
{
	WSA()
	{ 
		int a = WSAStartup(MAKEWORD(2,2), &_wsa); 
		assert(a == NO_ERROR);
	}
	~WSA() { WSACleanup(); }
	WSADATA _wsa;
} WSAInitializer;
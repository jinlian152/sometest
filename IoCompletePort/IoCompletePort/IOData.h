#pragma once
#include "Typedef.h"

enum IODataType
{
	IO_TYPE_READ,
	IO_TYPE_WRITE,
};

class IOData : public OVERLAPPED
{
public:
	IOData(IODataType type)
	{
		memset(this, 0 ,sizeof(*this));
		type_ = type;
	}

	~IOData(void)
	{

	}

public:
	IODataType				type_;

private:
	IOData(void);
	IOData& operator = (IOData&);
};


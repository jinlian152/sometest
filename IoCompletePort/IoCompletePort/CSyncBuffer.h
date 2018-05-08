#pragma once
#include "Mutex.h"

class CSyncBuffer
{
public:
	CSyncBuffer(int size = 4096)
		: size_(size)
	{
		buffer_ = new char[size];
		memset(buffer_, 0, size);
		rptr_ = buffer_;
		wptr_ = buffer_;
	}
	~CSyncBuffer(void);

	int write(char *buffer, int len);
	int remove(int len);
	bool resize(int size);
	void restoration();
	char *getRead();
	char *getWrite();
	int getFreeSize();
	int getSize();
	int getUsedSize();
	void clear();

private:
	char				*buffer_;
	int					size_;
	char				begin_;
	char				end_;
	char				*wptr_;
	char				*rptr_;

	Mutex				mutex_;
};
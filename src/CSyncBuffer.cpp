#include "CSyncBuffer.h"


CSyncBuffer::~CSyncBuffer(void)
{
	if (NULL != buffer_)
	{
		delete []buffer_;
		buffer_ = NULL;
	}
	wptr_ = NULL;
	rptr_ = NULL;
	begin_ = NULL;
	end_ = NULL;
}


int CSyncBuffer::write(char *buffer, int len)
{
	AutoLock<Mutex> lock(&mutex_);
	if (getFreeSize() >= len)
	{
		memcpy(wptr_, buffer, len);
		wptr_ += len;
	}
	else
	{
		resize(size_ * 2);//TODO:
		memcpy(wptr_, buffer, len);
		wptr_ += len;
	}
	return len;
}
int CSyncBuffer::remove(int len)
{
	AutoLock<Mutex> lock(&mutex_);
	if (getUsedSize() >= len)
	{
		rptr_ += len;
		return len;
	}
	return 0;
}

bool CSyncBuffer::resize(int size)
{
	AutoLock<Mutex> lock(&mutex_);
	if (size > size_)
	{
		int len = getUsedSize();
		char *newBuffer = new char[size];
		memset(newBuffer, 0, size);
		memcpy(newBuffer, rptr_, len);
		delete [] buffer_;
		buffer_ = newBuffer;
		rptr_ = buffer_;
		wptr_ = rptr_ + len;
		size_ = size;
	}
	return true;
}

void CSyncBuffer::restoration()
{
	AutoLock<Mutex> lock(&mutex_);
	int len = getUsedSize();
	memmove(buffer_, rptr_, len);
	rptr_ = buffer_;
	wptr_ = rptr_ + len;
}

char* CSyncBuffer::getRead()
{
	return rptr_;
}
char* CSyncBuffer::getWrite()
{
	return wptr_;
}
int CSyncBuffer::getFreeSize()
{
	return (buffer_ + size_ - wptr_);
}
int CSyncBuffer::getSize()
{
	return size_;
}
int CSyncBuffer::getUsedSize()
{
	return (wptr_ - rptr_);
}

void CSyncBuffer::clear()
{
	AutoLock<Mutex> lock(&mutex_);
	memset(buffer_, 0, sizeof(buffer_));
	rptr_ = buffer_;
	wptr_ = rptr_;
}
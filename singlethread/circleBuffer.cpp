#include "circleBuffer.h"
#include <stdio.h>
CircleBuffer::CircleBuffer(int size)
	:size_(size)
{
	buffer_ = new char[size_];
	memset(buffer_, 0, DEF_BUFFER_SIZE);
	begin_	= buffer_;
	end_	= buffer_;
	tail_	= buffer_ + (size_ - 1);
}

CircleBuffer::~CircleBuffer(void)
{
	if (NULL != buffer_)
	{
		delete [] buffer_;
		buffer_ = NULL;
	}
}

int CircleBuffer::write(char *src, int size)
{
	if (getFreeSize() < size)
	{
		printf("The circle free space is not enough!\n");
		return 0;
	}

	int len = tail_ - end_;
	if (len > size)
	{
		memcpy(end_, src, size);
		end_ += size;
	}
	else
	{
		memcpy(end_, src, len);
		memcpy(buffer_, src + len, size - len);
		end_ = buffer_ + size -len;
	}
	return size;
}

int CircleBuffer::read(char *dest, int size)
{
	if (getUsedSize() < size)
		return 0;

	int len = (tail_ - begin_);
	if (len < size)
	{
		memcpy(dest, begin_, len);
		memcpy(dest + len, buffer_, size - len);
	}
	else
		memcpy(dest, begin_, size);
	return size;
}

char* CircleBuffer::getRead()
{
	return begin_;
}

void CircleBuffer::remove(int size)
{
	if (getUsedSize() < size)
		return;

	int len = (tail_ - begin_);
	if (len <= size)
		begin_ = buffer_ + (size - len);
	else
		begin_ += size;
}

bool CircleBuffer::isFull()
{
	return (1 == (size_ - getUsedSize()));
}

bool CircleBuffer::isEmpty()
{
	return (0 == getUsedSize());
}

int CircleBuffer::getFreeSize()
{
	return size_ - getUsedSize() -1;
}

int CircleBuffer::getUsedSize()
{
	return (end_ - begin_ + size_) % size_;
}

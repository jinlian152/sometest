#ifndef DEF_CircleBuffer_INCLUDE
#define DEF_CircleBuffer_INCLUDE

#define DEF_BUFFER_SIZE				4096
#include <math.h>
#include <string.h>

class CircleBuffer
{
public:

	CircleBuffer(int size = DEF_BUFFER_SIZE);
	~CircleBuffer(void);

	int write(char *src, int size);
	int read(char *dest, int size);
	char *getRead();
	void remove(int size);
	bool isFull();
	bool isEmpty();
	int getFreeSize();
	int getUsedSize();

private:
	char			*buffer_;
	char			*tail_;
	int				size_;
	char			*begin_;
	char			*end_;
};

#endif
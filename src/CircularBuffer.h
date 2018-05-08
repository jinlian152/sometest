#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

class CircularBuffer
{
public:
	CircularBuffer(int size);
	virtual ~CircularBuffer();

	int GetFreeSpaceSize() const { return _size - _asize - _bsize; }
	int GetUsedSpaceSize() const { return _asize + _bsize; } 

	void Write(const char* src, int size);
	void Read(char* dest, int size) const;
	void Remove(int size);

private:
	int _GetFreeSpaceSizeAfterA() const { return _size - (_a - _base) - _asize; }
	
private:
	char *_base;	// pointor to memory buffer base
	int _size;		// total size of memory buffer 

	char *_a;		// pointor to A buffer
	int _asize;		// size of A buffer 

	char *_b;		// pointor to B buffer
	int _bsize;		// size of B buffer

#ifdef _DEBUG
	char *_a0;		// backup pointor to A buffer
	int _asize0;	// backup size of A buffer 
#endif
};

#endif

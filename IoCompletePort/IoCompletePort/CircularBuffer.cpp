#include "stdafx.h"
#include "CircularBuffer.h"
#include <assert.h>

CircularBuffer::CircularBuffer(int size)
:_base(NULL), _size(size), _a(NULL), _asize(0), _b(NULL), _bsize(0)
{
	_base = new char[_size];
	_a = _base;
}


CircularBuffer::~CircularBuffer()
{
	delete [] _base;
}


void CircularBuffer::Write(const char* src, int size)
{
	assert(size <= GetFreeSpaceSize());

	// if B exists, save in B 
	if (_b != NULL) 
	{
		memcpy(_b + _bsize, src, size);
		_bsize += size;
		return;
	}

	// only save in A
	if (size <= _GetFreeSpaceSizeAfterA()) 
	{
		memcpy(_a + _asize, src, size); 
		_asize += size;
		return;
	}

	// save in A and B
	int size_in_A = _GetFreeSpaceSizeAfterA();
	int size_in_B = size - size_in_A;
	int offset = size_in_A;
	
	memcpy(_a + _asize, src, size_in_A);
	_asize += size_in_A; 

	_b = _base; // initialize B
	memcpy(_b, src + offset, size_in_B);
	_bsize = size_in_B;
}


void CircularBuffer::Read(char* dest, int size) const
{
	//assert(size <= GetUsedSpaceSize());

	if (size <= _asize) // only read in A
	{
		memcpy(dest, _a, size);
	}
	else // read in A and B
	{
		memcpy(dest, _a, _asize);
		memcpy(dest + _asize, _b, size - _asize);
	}
}


void CircularBuffer::Remove(int size)
{
	assert(size <= GetUsedSpaceSize());
#ifdef _DEBUG
	_a0 = _a;
	_asize0 = size;
#endif

	if (size <= _asize) // only read in A
	{
		_a += size;
		_asize -= size;
	}
	else // read in A and B
	{
		int left_size = size - _asize;
		_b += left_size;
		_bsize -= left_size;
		_asize = 0;
	}

	// A is empty, so switch A to B
	if (_asize == 0)
	{

		if (_b != NULL) // B buffer exists
		{
			_a = _b;
			_asize = _bsize;
			_b = NULL;
			_bsize = 0;
		}
		else // no B buffer
		{
			_a = _base;
			_asize = 0;
		}
	}
}
#pragma once
#include "Mutex.h"
#include <deque>

template <class T> 
class CSyncQueue
{
public:
	CSyncQueue();
	~CSyncQueue();
	
	int size();
	bool isEmpty();
	void enqueue(const T &item);
	int deEnqueue(T &item);
	int front(T &item);

private:
	std::deque<T>		items_;
	Mutex				mutex_;
};

template <class T>
CSyncQueue<T>::CSyncQueue()
{
}

template <class T>
CSyncQueue<T>::~CSyncQueue()
{
}

template <class T>
int CSyncQueue<T>::size()
{
	return items_.size();
}

template <class T>
bool CSyncQueue<T>::isEmpty()
{
	return items_.empty();
}

template <class T>
void CSyncQueue<T>::enqueue(const T &item)
{
	mutex_.lock();
	items_.push_back(item);
	mutex_.unlock();
}

template <class T>
int CSyncQueue<T>::deEnqueue(T &item)
{
	int rc = -1;
	mutex_.lock();
	if (!items_.empty())
	{
		item = items_.front();
		items_.pop_front();
		rc = 0;
	}
	mutex_.unlock();
	return rc;
}

template <class T>
int CSyncQueue<T>::front(T &item)
{
	int rc = -1;
	mutex_.lock();
	if (!items_.empty())
	{
		item = items_.front();
		rc = 0;
	}
	mutex_.unlock();
	return rc;
}
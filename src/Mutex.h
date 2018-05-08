#pragma once
#include "Typedef.h"

class Mutex
{
public:
	Mutex(void)
	{
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
		pthread_mutex_init(&mutex_lock, &attr);
	}
	~Mutex(void)
	{
		pthread_mutex_destroy(&mutex_lock);
	}

	void lock()
	{
		pthread_mutex_lock(&mutex_lock);
	}

	void unlock()
	{
		pthread_mutex_unlock(&mutex_lock);
	}

	void tryLock()
	{
		pthread_mutex_trylock(&mutex_lock);
	}

private:
	pthread_mutex_t   mutex_lock;
	pthread_mutexattr_t attr; 
};


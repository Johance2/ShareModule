#ifndef __CLockUnit_h__
#define __CLockUnit_h__
#include<assert.h>
#if defined WIN32
#include <windows.h>
class CLock {
public:
	CLock() {
		::InitializeCriticalSection(&m_lock);
	}

	~CLock() {
		::DeleteCriticalSection(&m_lock);
	}

	void Clear() {
		::DeleteCriticalSection(&m_lock);
		::InitializeCriticalSection(&m_lock);
	}

	void Lock() {
		::EnterCriticalSection(&m_lock);
	}

	void UnLock() {
		::LeaveCriticalSection(&m_lock);
	}

	bool TryLock() {
		::TryEnterCriticalSection(&m_lock);
	}

private:
	CRITICAL_SECTION m_lock;
};
#else
// linux
#include <pthread.h>
class CLock {
public:

	CLock() {
		pthread_mutex_init(&m_lock, NULL);
	}

	~CLock() {
		pthread_mutex_destroy(&m_lock);
	}

	void Clear() {
		::pthread_mutex_destroy(&m_lock);
		::pthread_mutex_init(&m_lock, NULL);
	}

	void Lock() {
		pthread_mutex_lock(&m_lock);
	}

	void UnLock() {
		pthread_mutex_unlock(&m_lock);
	}

	bool TryLock() {
		return !pthread_mutex_trylock(&m_lock);
	}

private:
	pthread_mutex_t m_lock;
};
#endif //defined WIN32 || defined WIN64

class CAutoLock {
public:
	CAutoLock(CLock * pLock) {
		assert(pLock != NULL);
		m_pLock = pLock;
		m_pLock->Lock();
	}

	void Free() {
		assert(m_pLock != NULL);
		m_pLock->UnLock();
	}

	~CAutoLock() {
		m_pLock->UnLock();
	}

private:
	CLock * m_pLock;
};

#endif //defined  __CLockUnit_h__


#include "CThread.h"
#include <assert.h>

#ifdef WIN32
#include <Windows.h>
#include <process.h>
#define THREAD_FUN unsigned int
#define ThreadID DWORD

static THREAD_FUN __stdcall ThreadProc(LPVOID lpParam) {
	CThread * pThis = (CThread *)lpParam;
	assert(pThis);
	pThis->Run();
	_endthreadex(0);
	return 0;
}
#else
static THREAD_FUN ThreadProc(void * pParam) {
	CThread * pThis = (CThread *)pParam;
	TASSERT(pThis, "a null thread owner point");
	pThis->Run();
	return pThis;
}
#endif

CThread::CThread() {
	m_hThread = 0;
}

CThread::~CThread() {

}

bool CThread::Start(int threadcount) {
	for (int i=0; i<threadcount; i++) {
#ifdef WIN32
		unsigned long long ret = _beginthreadex(NULL, 0, ThreadProc, (void *) this, 0, NULL);
		if (ret == -1 || ret == 1  || ret == 0) {
			return false;
		}
		m_hThread = ret;
#else
		pthread_t ptid = 0;
		int ret = pthread_create(&ptid, NULL, ThreadProc, (void*)this);
		if (ret != 0) {
			return false;
		}
		m_hThread = ptid;
#endif
	}

	return true;
}

bool CThread::Wait() {
#ifdef WIN32
	if (WaitForSingleObject((HANDLE)m_hThread, INFINITE) != WAIT_OBJECT_0) {
		return false;
	}
#else
	if (pthread_join((pthread_t)m_hThread, NULL) != 0) {
		return false;
	}
#endif
	return true;
}

bool CThread::Terminate() {
#ifdef WIN32
	return TerminateThread((HANDLE)m_hThread, 1) ? true :false;
#else
	return pthread_cancel((pthread_t)m_hThread) == 0 ? true : false;
#endif
}

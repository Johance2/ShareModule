#include <Windows.h>
#include "MiniDump.h"

#include <time.h>
#include <DbgHelp.h>
#pragma comment( lib, "DbgHelp")


LONG CMiniDump::UnHandleExceptionFilterS(PEXCEPTION_POINTERS pExceptionInfo)
{
	std::wstring strFileName;
	MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
	HANDLE hDumpFile = INVALID_HANDLE_VALUE;
	
	
	if (!GetDumpFileName(strFileName))
		return EXCEPTION_EXECUTE_FAULT;

	// 写dump文件
	hDumpFile = ::CreateFile(strFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDumpFile == INVALID_HANDLE_VALUE)
		return EXCEPTION_EXECUTE_FAULT;
	loExceptionInfo.ExceptionPointers = pExceptionInfo;
	loExceptionInfo.ThreadId = ::GetCurrentThreadId();
	loExceptionInfo.ClientPointers = TRUE;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
						MiniDumpNormal, &loExceptionInfo, NULL, NULL);
	CloseHandle(hDumpFile);

	return EXCEPTION_EXECUTE_HANDLER;
}

CMiniDump::CMiniDump()
{
	pOldExceptionInfo = NULL;
    InstallDump();
}

CMiniDump::~CMiniDump()
{
    UnInstallDump();
}

BOOL CMiniDump::InstallDump()
{
	pOldExceptionInfo = ::SetUnhandledExceptionFilter(UnHandleExceptionFilterS);
	return TRUE;
}

BOOL CMiniDump::UnInstallDump()
{
	::SetUnhandledExceptionFilter(pOldExceptionInfo);
	return TRUE;
}

BOOL CMiniDump::GetDumpFileName(std::wstring& strDump)
{
	time_t timeExcep;
	TCHAR szTime[64] = {0}, szPath[MAX_PATH] = {0};
	TCHAR *pTmp;


	// 创建dump文件夹
	::GetModuleFileName(NULL, szPath, MAX_PATH * sizeof(TCHAR));
    pTmp = wcschr(szPath, '\\');
	if (pTmp != NULL)
		*pTmp = '\0';
	strDump = pTmp;
	strDump += L"Dump\\";
	CreateDirectory(strDump.c_str(), NULL);


	// 获取时间，组成dump文件名
	timeExcep = time(NULL);
	 wcsftime(szTime, sizeof(szTime), L"%Y%m%d%H%M%S", localtime(&timeExcep));
	 strDump += L"DumpFile_";
	 strDump += szTime;
	 strDump += L"_App";
	 strDump += L".dmp";

	return TRUE;
}
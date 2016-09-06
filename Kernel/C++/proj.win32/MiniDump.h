#pragma once

#include <string>

class CMiniDump
{
public:
	CMiniDump();
	~CMiniDump();
	static LONG WINAPI UnHandleExceptionFilterS(PEXCEPTION_POINTERS pExceptionInfo);

public:
	BOOL InstallDump();
	BOOL UnInstallDump();

private:
	static BOOL GetDumpFileName(std::wstring& strDump);

private:
	LPTOP_LEVEL_EXCEPTION_FILTER pOldExceptionInfo;

};
#ifndef _IKernel_H_
#define _IKernel_H_
#include "IModule.h"
#include "IDataStream.h"

#ifdef _WIN32
#include <windows.h>
typedef HMODULE MODULE_HANDLE;
#else
#endif

class IKernel
{
public:
	virtual IModule* FindModule(UCHAR ucID) = 0;	
	virtual MODULE_HANDLE FindModuleHandle(UCHAR ucID) = 0;	
	virtual void Quit() = 0;
	virtual IDataStream *GetFileData(const char *pFileName) = 0;
	virtual INT64 GetCuttentTime() = 0;
	virtual void ConvertUtf8ToGBK(std::string& strUtf8) = 0;
	virtual void ConvertGBKToUtf8(std::string& strGBK) = 0;
	virtual std::wstring ConvertUtf8ToWideChar(const std::string& strUtf8) = 0;
	virtual std::string ConvertWideCharToUtf8(const std::wstring& strWideChar) = 0;

	// log4z interface
public:
	virtual void log_stream(int id, int level, const char *file, int line, const char *log) = 0;
	virtual void log_format(int id, int level, const char *file, int line, const char *logformat, ...) = 0;

};
extern IKernel *g_pKernel;

#define MODULE_INTERFACE(className) \
IKernel *g_pKernel = NULL;\
extern "C" __declspec(dllexport) IModule *CreateModule(IKernel *pKernel)\
{\
	g_pKernel = pKernel;\
	return new className##();\
}\
extern "C" __declspec(dllexport) void DestroyModule(IModule *pModule)\
{\
	delete pModule;\
}

#endif

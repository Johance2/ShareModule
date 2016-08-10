#ifndef _IKernel_H_
#define _IKernel_H_
#include "IModule.h"
#include "IDataStream.h"

class IKernel
{
public:
	virtual IModule* FindModule(UCHAR ucID) = 0;
	virtual IDataStream *GetFileData(const char *pFileName) = 0;
	virtual INT64 GetCuttentTime() = 0;

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

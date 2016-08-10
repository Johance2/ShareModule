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
};

#endif
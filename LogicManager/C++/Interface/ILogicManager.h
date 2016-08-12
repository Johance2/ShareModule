#ifndef __ILogicManager_H__
#define __ILogicManager_H__

#include "IKernel.h"

#define LogicManagerModuleID 129


class ILogicManager : public IModule
{
public:
	virtual void Init(ModuleAttribute *pAttribute) = 0;
	virtual void Shut() = 0;
	virtual void Update(float fTime) = 0;
	virtual UCHAR GetModuleID() {return LogicManagerModuleID;};

public:
};

extern ILogicManager *g_pLogicManager;

#endif // __ILogicManager_H__
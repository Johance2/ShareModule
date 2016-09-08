#ifndef __IPushBox_H__
#define __IPushBox_H__

#include "IKernel.h"

#define PushBoxModuleID 131


class IPushBox : public IModule
{
public:
	virtual void Init(ModuleAttribute *pAttribute) = 0;
	virtual void Shut() = 0;
	virtual void Update(float fTime) = 0;
	virtual UCHAR GetModuleID() {return PushBoxModuleID;};

public:
};

extern IPushBox *g_pPushBox;

#endif // __IPushBox_H__
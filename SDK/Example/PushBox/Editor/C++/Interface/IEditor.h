#ifndef __IEditor_H__
#define __IEditor_H__

#include "IKernel.h"

#define EditorModuleID 131


class IEditor : public IModule
{
public:
	virtual void Init(ModuleAttribute *pAttribute) = 0;
	virtual void Shut() = 0;
	virtual void Update(float fTime) = 0;
	virtual UCHAR GetModuleID() {return EditorModuleID;};

public:
};

extern IEditor *g_pEditor;

#endif // __IEditor_H__
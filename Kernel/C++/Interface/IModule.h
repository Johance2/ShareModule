#ifndef _IModule_H_
#define _IModule_H_
#include "BaseType.h"
// ModuleID 0-128 系统模块使用
// 129-255 为用户新增模块ID使用

class IModule
{
public:
	virtual void Init() = 0;
	virtual void Shut() = 0;
	virtual void Update(float fTime) = 0;
	virtual UCHAR GetModuleID() = 0;
};

#endif
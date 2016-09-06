#ifndef _IModule_H_
#define _IModule_H_
#include "BaseType.h"
// ModuleID 0-128 ϵͳģ��ʹ��
// 129-255 Ϊ�û�����ģ��IDʹ��
typedef std::map<std::string, std::string> ModuleAttribute;

class IModule
{
public:
	virtual void Init(ModuleAttribute *pAttribute) = 0;
	virtual void Shut() = 0;
	virtual void Update(float fTime) = 0;
	virtual UCHAR GetModuleID() = 0;
};

#endif
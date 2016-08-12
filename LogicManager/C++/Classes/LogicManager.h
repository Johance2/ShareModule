#ifndef __LogicManager_H__
#define __LogicManager_H__

#include "../Interface/ILogicManager.h"

class LogicManager : public ILogicManager
{
public:
    LogicManager();
    ~LogicManager();
	
	virtual void Init(ModuleAttribute *pAttribute);
	virtual void Shut();
	virtual void Update(float fTime);
	
public:
	
private:
};

#define g_pLogicManager ((LogicManager*)GameEnv::GetGameModule(eGMIT_LogicManager))

#endif // __LogicManager_H__
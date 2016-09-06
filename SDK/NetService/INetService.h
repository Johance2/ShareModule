#ifndef __INetService_H__
#define __INetService_H__
#include "NFINet.h"
#include "IKernel.h"

#define NetServiceModuleID 1

class INetService : public IModule
{
public:
	virtual void Init(ModuleAttribute *pAttribute) = 0;
	virtual void Shut() = 0;
	virtual void Update(float fTime) = 0;

public:
	virtual NFINet* CreateClient(NET_RECEIVE_FUNCTOR handleRecieve, NET_EVENT_FUNCTOR handleEvent, const char* strIP, const unsigned short nPort) = 0;
	virtual NFINet* CreateServer(NET_RECEIVE_FUNCTOR handleRecieve, NET_EVENT_FUNCTOR handleEvent, const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) = 0;
};

extern INetService *g_pNetService;

#endif // __INetService_H__
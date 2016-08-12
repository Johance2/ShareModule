#ifndef __NetService_H__
#define __NetService_H__

#include "../Interface/INetService.h"
#include "NFCNet.h"
#include <list>

class NetService : public INetService
{
public:
    NetService();
    ~NetService();
	
	virtual void Init(ModuleAttribute *pAttribute);
	virtual void Shut();
	virtual void Update(float fTime);
	virtual UCHAR GetModuleID() { return NetServiceModuleID; }
	
public:
	virtual NFINet* CreateClient(NET_RECEIVE_FUNCTOR handleRecieve, NET_EVENT_FUNCTOR handleEvent, const char* strIP, const unsigned short nPort);
	virtual NFINet* CreateServer(NET_RECEIVE_FUNCTOR handleRecieve, NET_EVENT_FUNCTOR handleEvent, const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4);

private:
	std::list<NFINet*> m_listNet;
};

#endif // __NetService_H__
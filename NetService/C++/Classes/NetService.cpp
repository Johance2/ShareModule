#include "NetService.h"

NetService::NetService(IKernel *pKernel)
{
	m_pKernel = pKernel;
}

NetService::~NetService()
{
}

void NetService::Init()
{
}

void NetService::Shut()
{
	std::list<NFINet*>::iterator it = m_listNet.begin();
	while(it != m_listNet.end())
	{
		(*it)->Final();
		delete (*it);
		++it;
	}
	m_listNet.clear();
}

void NetService::Update(float fTime)
{
	std::list<NFINet*>::iterator it = m_listNet.begin();
	while(it != m_listNet.end())
	{
		(*it)->Execute();
		++it;
	}
}

NFINet* NetService::CreateClient(NET_RECEIVE_FUNCTOR handleRecieve, NET_EVENT_FUNCTOR handleEvent, const char* strIP, const unsigned short nPort)
{
	NFCNet *pNet = new NFCNet(handleRecieve, handleEvent);

	pNet->Initialization(strIP, nPort);

	m_listNet.push_back(pNet);

	return pNet;
}

NFINet* NetService::CreateServer(NET_RECEIVE_FUNCTOR handleRecieve, NET_EVENT_FUNCTOR handleEvent, const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount/* = 4*/)
{
	NFCNet *pNet = new NFCNet(handleRecieve, handleEvent);

	pNet->Initialization(nMaxClient, nPort, nCpuCount);
	
	m_listNet.push_back(pNet);

	return pNet;
}
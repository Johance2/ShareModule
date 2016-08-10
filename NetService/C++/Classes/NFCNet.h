#ifndef NFC_NET_H
#define NFC_NET_H
#include "../Interface/NFINet.h"

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/thread.h>
#include <event2/event_compat.h>

#pragma pack(push, 1)

typedef unsigned int unint32_t;

class NFCNet : public NFINet
{
public:
    NFCNet()
    {
        base = NULL;
        listener = NULL;

        mstrIP = "";
        mnPort = 0;
        mnCpuCount = 0;
        mbServer = false;
    }

    NFCNet(NET_RECEIVE_FUNCTOR handleRecieve, NET_EVENT_FUNCTOR handleEvent)
    {
        base = NULL;
        listener = NULL;

        mRecvCB = handleRecieve;
        mEventCB = handleEvent;
        mstrIP = "";
        mnPort = 0;
        mnCpuCount = 0;
        mbServer = false;
    }
    virtual ~NFCNet() {};

public:
    virtual bool Execute();

    virtual void Initialization(const char* strIP, const unsigned short nPort);
    virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4);

    virtual bool Final();

    //无包头，内部组装
    virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const int nSockIndex);

    //无包头，内部组装
    virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const std::list<int>& fdList);

    //无包头，内部组装
    virtual bool SendMsgToAllClientWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen);


    virtual bool CloseNetObject(const int nSockIndex);
    virtual bool AddNetObject(const int nSockIndex, NetObject* pObject);
    virtual NetObject* GetNetObject(const int nSockIndex);

    virtual bool IsServer();
    virtual bool Log(int severity, const char* msg);

private:
    //已带上包头
    bool SendMsgToAllClient(const char* msg, const uint32_t nLen);

    //已带上包头
    bool SendMsg(const char* msg, const uint32_t nLen, const std::list<int>& fdList);

    //已带上包头
    bool SendMsg(const char* msg, const uint32_t nLen, const int nSockIndex);

private:
    void ExecuteClose();
    bool CloseSocketAll();

    bool Dismantle(NetObject* pObject);


    int InitClientNet();
    int InitServerNet();
    void CloseObject(const int nSockIndex);

    static void listener_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int socklen, void* user_data);
    static void conn_readcb(struct bufferevent* bev, void* user_data);
    static void conn_writecb(struct bufferevent* bev, void* user_data);
    static void conn_eventcb(struct bufferevent* bev, short events, void* user_data);
    static void log_cb(int severity, const char* msg);

protected:
    int DeCode(const char* strData, const uint32_t unLen, NFCMsgHead& xHead);
    int EnCode(const uint16_t unMsgID, const char* strData, const uint32_t unDataLen, std::string& strOutData);

private:
    //<fd,object>


    std::map<int, NetObject*> mmObject;
    std::vector<int> mvRemoveObject;

    int mnMaxConnect;
    std::string mstrIP;
    int mnPort;
    int mnCpuCount;
    bool mbServer;

    struct event_base* base;
    struct evconnlistener* listener;
    //////////////////////////////////////////////////////////////////////////

    NET_RECEIVE_FUNCTOR mRecvCB;
    NET_EVENT_FUNCTOR mEventCB;

    //////////////////////////////////////////////////////////////////////////
};

#pragma pack(pop)

#endif

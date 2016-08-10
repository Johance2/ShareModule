#pragma once

#include "../Interface/ISQLService.h"
#include "CThread.h"
#include "CLock.h"

#include <map>
#include <list>
#include <functional>


class SQL_Request
{
public:
	SQL_Request(const char *cmd, SQL_Callback callback, SQLParam *param, bool bHasResult)
	{
		strCmd = cmd;
		_callback = callback;
		_param = param;
		bHasResult = bHasResult;
	}
	std::string strCmd;
	SQL_Callback _callback;
	SQLParam *_param;
	bool _bHasResult ;
};

class SQL_Respon
{
public:
	SQL_Respon(SQL_Callback callback, IQueryResult *result, bool retCode)
	{
		_result = result;
		_callback = callback;
		errorCode = retCode;
	}
	SQL_Callback _callback;
	IQueryResult *_result;
	int errorCode;
};

class DataBase : public ISQLService, public CThread
{
public:
	DataBase() {}
    ~DataBase(void) {};

	virtual void Update(float fTime);
    
    virtual int Open(const char * constr);

    virtual int ExcuteQuery(IQueryResult **result, const char *cmd) = 0;
    virtual int ExcuteQueryf(IQueryResult **result, const char *cmd, ...);
    virtual int ExcuteQuery(const char *cmd) = 0;
    virtual int ExcuteQueryf(const char *cmd, ...);
    
    virtual int ExcuteBinary(const char *cmd, SQLParam *param) = 0;
    virtual int ExcuteBinaryf(const char *cmd, SQLParam *param, ...);
		
    virtual void ExcuteQueryAsync(const char *cmd, SQL_Callback callback = 0, bool bHasResult = true);
    virtual void ExcuteQueryAsyncf(const char *cmd, SQL_Callback callback = 0, bool bHasResult = true, ...);
    virtual void ExcuteBinaryAsync(const char *cmd, SQLParam *param, SQL_Callback callback = 0);
    virtual void ExcuteBinaryAsyncf(const char *cmd, SQLParam *param, SQL_Callback callback = 0, ...);

	virtual void Run();

protected:
    virtual int OpenImpl(std::map<std::string, std::string> &strmap) = 0;
    char _param[1024];

private:
	CLock m_RequestLock;
	std::list<SQL_Request*> m_listRequest;

	CLock m_ResponLock;
	std::list<SQL_Respon*> m_listRespon;
};

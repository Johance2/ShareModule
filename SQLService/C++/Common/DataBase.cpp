#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "DataBase.h"
#include <stdarg.h>
#include "CLock.h"

void DataBase::Update(float fTime)
{
	int nResponCount = 0;
	do
	{
		SQL_Respon *pRespon = NULL;
		{
			CAutoLock guard(&m_ResponLock);
			nResponCount = m_listRespon.size();
			if(nResponCount == 0)
				break;
			pRespon = m_listRespon.front();
			m_listRespon.pop_front();
		}
		pRespon->_callback(pRespon->_result, pRespon->errorCode);
		nResponCount--;
	}while(nResponCount);
}

int DataBase::Open(const char * constr)
{
    char *p = _strdup(constr);

    char *tmp = p;

    char *param;
    char *val;

    std::map<std::string, std::string> strmap;

    tmp = strtok(tmp, "=");
    while(tmp)
    {
        param = tmp;
        tmp = strtok(0, ";");
        if(!tmp)
            break;
        val = tmp;
        strmap[param] = val;
        tmp = strtok(0, "=");
    }

    free(p);
    return OpenImpl(strmap);
}

int DataBase::ExcuteQueryf(IQueryResult **result, const char *cmd, ...)
{
    va_list arg_ptr;

    va_start(arg_ptr, cmd);
    vsprintf(_param, cmd, arg_ptr);

    va_end(arg_ptr);
    return ExcuteQuery(result, _param);
}

int DataBase::ExcuteQueryf(const char *cmd, ...)
{
    va_list arg_ptr;

    va_start(arg_ptr, cmd);
    vsprintf(_param, cmd, arg_ptr);

    va_end(arg_ptr);
    return ExcuteQuery(_param);
}


int DataBase::ExcuteBinaryf(const char *cmd, SQLParam *param, ...)
{
    va_list arg_ptr;

    va_start(arg_ptr, cmd);
    vsprintf(_param, cmd, arg_ptr);

    va_end(arg_ptr);
    return ExcuteBinary(_param, param);
}

void DataBase::ExcuteQueryAsync(const char *cmd, SQL_Callback callback, bool bHasResult/* = true*/)
{
	CAutoLock guard(&m_RequestLock);

	SQL_Request *pRequest = new SQL_Request(cmd, callback, NULL, bHasResult);
	m_listRequest.push_back(pRequest);
}

void DataBase::ExcuteQueryAsyncf(const char *cmd, SQL_Callback callback, bool bHasResult/*t = true*/, ...)
{
    va_list arg_ptr;

    va_start(arg_ptr, cmd);
    vsprintf(_param, cmd, arg_ptr);

    va_end(arg_ptr);

    ExcuteQueryAsync(cmd, callback, bHasResult);
}

void DataBase::ExcuteBinaryAsync(const char *cmd, SQLParam *param, SQL_Callback callback)
{
	CAutoLock guard(&m_RequestLock);

	SQL_Request *pRequest = new SQL_Request(cmd, callback, param, false);
	m_listRequest.push_back(pRequest);
}

void DataBase::ExcuteBinaryAsyncf(const char *cmd, SQLParam *param, SQL_Callback callback, ...)
{
    va_list arg_ptr;

    va_start(arg_ptr, cmd);
    vsprintf(_param, cmd, arg_ptr);

    va_end(arg_ptr);

    ExcuteBinaryAsync(cmd, param, callback);
}

void DataBase::Run()
{
	while(true)
	{
		CAutoLock guard(&m_RequestLock);
		if(m_listRequest.size() > 0)
		{
			m_RequestLock.Lock();
			SQL_Request *pRequest = m_listRequest.front();
			m_listRequest.pop_front();			
			m_RequestLock.UnLock();

			SQL_Respon *pRespon = NULL;
			if(pRequest->_param)
			{
				int nRet = ExcuteBinary(pRequest->strCmd.c_str(), pRequest->_param);
				pRespon = new SQL_Respon(pRequest->_callback, NULL, nRet);
			}
			else if(pRequest->_bHasResult)
			{
				IQueryResult *pResult;
				int nRet = ExcuteQuery(&pResult, pRequest->strCmd.c_str());
				pRespon = new SQL_Respon(pRequest->_callback, pResult, nRet);
			}
			else
			{
				int nRet = ExcuteQuery(pRequest->strCmd.c_str());
				pRespon = new SQL_Respon(pRequest->_callback, NULL, nRet);
			}
			m_ResponLock.Lock();
			m_listRespon.push_back(pRespon);
			m_ResponLock.UnLock();
		}
	}
}
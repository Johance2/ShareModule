/********************************************************************
	purpose:	数据库服务接口实现类
*********************************************************************/

#ifndef _ODBCService_H_
#define _ODBCService_H_
#include "../../Common/DataBase.h"

#include <windows.h>
#include "sql.h"
#include "sqlext.h"

class ODBCService :
	public DataBase
{
public:
	ODBCService();
	virtual ~ODBCService(void);

	// --------- 实现基类接口 ------------
	virtual void Init();
	virtual void Shut();
	virtual void Update(float fTime);

public:
    int ExcuteQuery(IQueryResult **result, const char *cmd);
    int ExcuteQuery(const char *cmd);
    int ExcuteBinary(const char *cmd, SQLParam *param);

protected:
    int OpenImpl(std::map<std::string, std::string> &strmap);
	
private:
	std::string mSourceName;
	std::string mUserName;
	std::string mPwd;
	SQLHANDLE hEnv;
	SQLHANDLE hDbc;
	SQLHSTMT  hstmt;
};

#endif

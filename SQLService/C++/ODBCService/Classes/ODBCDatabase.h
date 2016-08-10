#pragma once
#include "Common.h"
#include "Database.h"
#include "sql.h"
#include "sqlext.h"

class MYDLL_API ODBCDatabase : public DataBase
{
    friend class DataBase;
public:
    ~ODBCDatabase(void);
    static DataBase* Create();
public:
    int ExcuteQuery(QueryResult **result, const char *cmd);
    int ExcuteQuery(const char *cmd);
    int ExcuteBinary(const char *cmd, SqlParam *param);

protected:
    ODBCDatabase(void);
    int OpenImpl(std::map<std::string, std::string> &strmap);

private:
	std::string mSourceName;
	std::string mUserName;
	std::string mPwd;
	SQLHANDLE hEnv;
	SQLHANDLE hDbc;
	SQLHSTMT  hstmt;
};

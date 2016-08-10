#include "StdAfx.h"
#include "ODBCDatabase.h"
#include "ByteBuffer.h"
#include "ODBCQueryResult.h"

ODBCDatabase::ODBCDatabase(void)
{
	hEnv = 0;
	hstmt = 0;
	hDbc = 0;
}

ODBCDatabase::~ODBCDatabase(void)
{
	if (hstmt)	
	{
		SQLCloseCursor(hstmt);
		SQLFreeStmt(hstmt, SQL_UNBIND);
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		hstmt = 0;
	}
	if (hDbc)
	{
		SQLFreeHandle(SQL_HANDLE_DESC, hDbc);
		SQLDisconnect(hDbc);
		hDbc = 0;
	}
	if (hEnv)	
	{
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		hEnv = 0;
	}
}

DataBase* ODBCDatabase::Create()
{
	return new ODBCDatabase();
}

int ODBCDatabase::OpenImpl(std::map<std::string, std::string> &strmap)
{
	mSourceName = strmap["Source"];
	mUserName = strmap["user"];
	mPwd = strmap["passwd"];

	SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&hEnv);
	SQLSetEnvAttr(hEnv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3,SQL_IS_INTEGER);
	SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&hDbc);
	SQLRETURN mResult=SQLConnectA(hDbc,(SQLCHAR *)mSourceName.c_str(), SQL_NTS,(SQLCHAR *)mUserName.c_str(),SQL_NTS,
		(SQLCHAR *)mPwd.c_str(),SQL_NTS);

	if ((mResult != SQL_SUCCESS) && (mResult != SQL_SUCCESS_WITH_INFO)) 
	{
		return mResult;
	}

	mResult = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hstmt);

	if ((mResult != SQL_SUCCESS) && (mResult != SQL_SUCCESS_WITH_INFO)) 
	{
		hstmt = 0;
		return mResult;
	}

    return mResult;
}

int ODBCDatabase::ExcuteQuery(QueryResult **result, const char *cmd)
{
	SQLRETURN mResult = SQLExecDirectA(hstmt,(SQLCHAR *)cmd,SQL_NTS);

	if(mResult == SQL_SUCCESS)
	{
		SQLINTEGER mAffectCount;
		SQLSMALLINT mColCount;

		mAffectCount = SQLRowCount(hstmt, &mAffectCount);
		SQLRowCount(hstmt, &mAffectCount);
		SQLNumResultCols(hstmt, &mColCount);

		*result = new ODBCQueryResult(hstmt, mAffectCount, mColCount);
	}


	return mResult;
}

int ODBCDatabase::ExcuteQuery(const char *cmd)
{
	SQLRETURN mResult = SQLExecDirectA(hstmt,(SQLCHAR *)cmd,SQL_NTS);
	return mResult;
}

int ODBCDatabase::ExcuteBinary(const char *cmd, SqlParam *param)
{
	return 0;
}
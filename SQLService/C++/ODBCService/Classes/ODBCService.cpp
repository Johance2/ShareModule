/********************************************************************
	purpose:	数据库服务接口实现类
*********************************************************************/
#include "ODBCService.h"
#include "ODBCQueryResult.h"

//-----------------------------------------------------
// 构造
//-----------------------------------------------------
ODBCService::ODBCService()
	: DataBase()
{
	hEnv = 0;
	hstmt = 0;
	hDbc = 0;
}

//-----------------------------------------------------
// 析构
//-----------------------------------------------------
ODBCService::~ODBCService(void)
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

//-----------------------------------------------------
// 初始化
//-----------------------------------------------------
void ODBCService::Init(ModuleAttribute *pAttribute)
{

}

//-----------------------------------------------------
// 关闭
//-----------------------------------------------------
void ODBCService::Shut()
{

}

//-----------------------------------------------------
// 更新
//-----------------------------------------------------
void ODBCService::Update( float fTime )
{
	DataBase::Update(fTime);
}

int ODBCService::OpenImpl(std::map<std::string, std::string> &strmap)
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

int ODBCService::ExcuteQuery(IQueryResult **result, const char *cmd)
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

int ODBCService::ExcuteQuery(const char *cmd)
{
	SQLRETURN mResult = SQLExecDirectA(hstmt,(SQLCHAR *)cmd,SQL_NTS);
	return mResult;
}

int ODBCService::ExcuteBinary(const char *cmd, SQLParam *param)
{
	SQLRETURN retcode = SQLPrepareA(hstmt, (SQLCHAR*)cmd, SQL_NTS); 
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		SQLSMALLINT numParam;
		retcode = SQLNumParams(hstmt, &numParam);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			for(SQLSMALLINT i = 0; i < numParam; i++)
			{
				SQLINTEGER status = SQL_DATA_AT_EXEC;
				retcode = SQLBindParameter(hstmt, i+1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY, 0, 0, param[i].data, param[i].length, &status);
				if (retcode != SQL_SUCCESS)
				{
					return retcode;
				}
			}
			retcode = SQLExecute(hstmt);
		}
	}

    return retcode;
}
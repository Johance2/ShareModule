/********************************************************************
	purpose:	数据库服务接口实现类
*********************************************************************/
#include "SQLiteService.h"
#include "SQLiteQueryResult.h"

//-----------------------------------------------------
// 构造
//-----------------------------------------------------
SQLiteService::SQLiteService()
	: DataBase()
{
    _db = 0;
}

//-----------------------------------------------------
// 析构
//-----------------------------------------------------
SQLiteService::~SQLiteService(void)
{
   if(_db != 0)
       sqlite3_close(_db);
}

//-----------------------------------------------------
// 初始化
//-----------------------------------------------------
void SQLiteService::Init()
{

}

//-----------------------------------------------------
// 关闭
//-----------------------------------------------------
void SQLiteService::Shut()
{

}

//-----------------------------------------------------
// 更新
//-----------------------------------------------------
void SQLiteService::Update( float fTime )
{
	DataBase::Update(fTime);
}

int SQLiteService::OpenImpl(std::map<std::string, std::string> &strmap)
{
	return sqlite3_open(strmap["filename"].c_str(), &_db);
}

int SQLiteService::ExcuteQuery(IQueryResult **result, const char *cmd)
{
    *result = 0;
		
	sqlite3_stmt *stmt;
	int ret =sqlite3_prepare(_db, cmd, strlen(cmd), &stmt, 0);

	if(ret != SQLITE_OK)
		return ret;

	*result = new SQLiteQueryResult(stmt, sqlite3_data_count(stmt), sqlite3_column_count(stmt));
	
	return ret;
}

int SQLiteService::ExcuteQuery(const char *cmd)
{
	char *errMsg;
	
	int ret = sqlite3_exec(_db, cmd, 0, 0, &errMsg);

	if(ret != SQLITE_OK)
		free(errMsg);

	return ret;
}

int SQLiteService::ExcuteBinary(const char *cmd, SQLParam *param)
{
	sqlite3_stmt *stmt;
	int ret = 0;

    do
    {
		ret = sqlite3_prepare(_db, cmd, -1, &stmt, 0);
        if(ret != SQLITE_OK)
        {
            break;
        }

        uint32 count= sqlite3_bind_parameter_count(stmt);

        for(uint32 i = 0; i < count; i++)
        {
			ret = sqlite3_bind_blob(stmt, i+1, param[i].data, param[i].length, SQLITE_STATIC);
			if (ret != SQLITE_OK)
            {
                break;
            }
        }
		ret = sqlite3_step(stmt);

    }while(0);
	
    return ret;
}
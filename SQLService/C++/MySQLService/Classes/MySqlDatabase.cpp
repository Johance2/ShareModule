#include "StdAfx.h"
#include "MySqlDatabase.h"
#include "MysqlQueryResult.h"

MySqlDatabase::MySqlDatabase(void)
{
    _connection = mysql_init(0);
}

MySqlDatabase::~MySqlDatabase(void)
{
   if(_connection != 0)
       mysql_close(_connection);
}

DataBase* MySqlDatabase::Create()
{
	return new MySqlDatabase();
}

int MySqlDatabase::OpenImpl(std::map<std::string, std::string> &strmap)
{
    if(!mysql_real_connect(_connection,
                       strmap["host"].c_str(),
                       strmap["user"].c_str(),
                       strmap["passwd"].c_str(),
                       strmap["db"].c_str(),
                       atoi(strmap["port"].c_str()),
                       0,
                       0))
    {
        return mysql_errno(_connection);
    }

    return 0;
}

int MySqlDatabase::ExcuteQuery(QueryResult **result, const char *cmd)
{
    *result = 0;

    mysql_query(_connection, cmd);

    MYSQL_RES *res = mysql_store_result(_connection);

    if(res)
        *result = new MysqlQueryResult(res, res->row_count, res->field_count);

    return mysql_errno(_connection);
}

int MySqlDatabase::ExcuteQuery(const char *cmd)
{
    mysql_query(_connection, cmd);
    return mysql_errno(_connection);
}

int MySqlDatabase::ExcuteBinary(const char *cmd, SqlParam *param)
{
    MYSQL_BIND *bind = 0;
    int err = 0;
    MYSQL_STMT *stmt;

    do
    {
        stmt = mysql_stmt_init(_connection);
        if(!stmt)
            break;
        if(mysql_stmt_prepare(stmt, cmd, strlen(cmd)))
        {
            err = mysql_errno(_connection);
            break;
        }

        uint32 count = mysql_stmt_param_count(stmt);

        bind = new MYSQL_BIND[count];
        memset(bind, 0, sizeof(bind) * count);

        for(uint32 i = 0; i < count; i++)
        {
            bind[i].buffer = param[i].data; 
            bind[i].buffer_type = MYSQL_TYPE_BLOB;
            bind[i].length= 0;
            bind[i].buffer_length = param[i].length;
            bind[i].is_null= 0;
            if (mysql_stmt_bind_param(stmt, bind))
            {
                err = mysql_errno(_connection);
                break;
            }
        }

        if(!err)
        {
            if(mysql_stmt_execute(stmt))
            {
                err = mysql_errno(_connection);
            }
        }

    }while(0);
    
    if(stmt)
        mysql_stmt_close(stmt);

    delete [] bind;

    return err;
}
#pragma once
#include "Common.h"
#include "Database.h"

struct st_mysql;

class MYDLL_API MySqlDatabase : public DataBase
{
    friend class DataBase;
public:
    ~MySqlDatabase(void);
    static DataBase* Create();
public:
    int ExcuteQuery(QueryResult **result, const char *cmd);
    int ExcuteQuery(const char *cmd);
    int ExcuteBinary(const char *cmd, SqlParam *param);

protected:
    MySqlDatabase(void);
    int OpenImpl(std::map<std::string, std::string> &strmap);

private:
    st_mysql *_connection;
};

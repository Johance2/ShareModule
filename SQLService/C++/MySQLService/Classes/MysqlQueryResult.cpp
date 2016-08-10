#include "MysqlQueryResult.h"
#pragma comment(lib, "libmySQL.lib")

MysqlQueryResult::MysqlQueryResult(MYSQL_RES *res, uint64 row_count, uint32 field_count)
: QueryResult(row_count, field_count)
, _mysql_res(res)
{
    _begin = mysql_row_tell(_mysql_res);
}

MysqlQueryResult::~MysqlQueryResult(void)
{
    if(_mysql_res)
        mysql_free_result(_mysql_res);
}

bool MysqlQueryResult::Read()
{
    _row = mysql_fetch_row(_mysql_res);
    if(_row)
    {
        mysql_fetch_lengths(_mysql_res);
        return true;
    }
    return false;
}

void MysqlQueryResult::SetToBegin()
{
    mysql_row_seek(_mysql_res, _begin);
}

std::string MysqlQueryResult::field_name(int n)
{
    return _mysql_res->fields[n].name;
}

uint32 MysqlQueryResult::length(int n)
{
    return _mysql_res->lengths[n];
}


uint32 MysqlQueryResult::get_data(int n, char *data)
{
    memcpy(data, _row[n], length(n));
    return length(n);
}

int32 MysqlQueryResult::get_int32(int n)
{
    return _row[n] ? strtol(_row[n], 0, 10) : 0;
}

int64 MysqlQueryResult::get_int64(int n)
{
    return _row[n] ? strtol(_row[n], 0, 10) : 0;
}

float MysqlQueryResult::get_float(int n)
{
    return _row[n] ? (float)atof(_row[n]) : 0;
}

double MysqlQueryResult::get_double(int n)
{
    return _row[n] ? atof(_row[n]) : 0;
}

std::string MysqlQueryResult::get_string(int n)
{
    return _row[n] ? _row[n] : "";
}

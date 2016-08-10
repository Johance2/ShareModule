#pragma once
#include "../../Common/QueryResult.h"
#include "./sqlite/sqlite3.h"

class SQLiteQueryResult :
    public QueryResult
{
public:
	SQLiteQueryResult(sqlite3_stmt *stmt, uint64 row_count, uint32 field_count);
    ~SQLiteQueryResult(void);

    virtual bool Read();
    virtual void SetToBegin();
    virtual std::string field_name(int n);
    virtual uint32 length(int32 n);

    virtual uint32 get_data(int n, char *data);
    virtual int32 get_int32(int n);
    virtual int64 get_int64(int n);
    virtual float get_float(int n);
    virtual double get_double(int n);
    virtual std::string get_string(int n);
private:
    sqlite3_stmt *_sqlite_stmt;
};

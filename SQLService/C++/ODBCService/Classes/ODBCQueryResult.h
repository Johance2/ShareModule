#pragma once
#include "../../Common/QueryResult.h"

#include <windows.h>
#include "sql.h"
#include "sqlext.h"

class ODBCQueryResult :
    public QueryResult
{
public:
	ODBCQueryResult(SQLHSTMT res, uint64 row_count, uint32 field_count);
    ~ODBCQueryResult(void);

    bool Read();
    void SetToBegin();
    std::string field_name(int n);
    uint32 length(int32 n);

    uint32 get_data(int n, char *data);
    int32 get_int32(int n);
    int64 get_int64(int n);
    float get_float(int n);
    double get_double(int n);
    std::string get_string(int n);
private:
	SQLHSTMT m_hstmt;
};

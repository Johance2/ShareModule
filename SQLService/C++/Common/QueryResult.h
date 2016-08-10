#pragma once
#include "../Interface/IQueryResult.h"

class QueryResult : public IQueryResult
{
public:
    QueryResult(uint64 row_count, uint32 field_count)
        :_row_count(row_count)
        ,_filed_count(field_count)
    {};

    ~QueryResult(void) {};

    uint64 row_count()
    {
        return _row_count;
    }

    uint32 field_count()
    {
        return _filed_count;
    }

protected:
    uint64 _row_count;
    uint32 _filed_count;
};

#pragma once
#include <string>

typedef __int8           int8;
typedef __int16          int16;
typedef __int32          int32;
typedef __int64          int64;

typedef unsigned __int8  uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned long    ulong;

class IQueryResult
{
public:
    virtual uint64 row_count() = 0;
    virtual uint32 field_count() = 0;
    virtual bool Read() = 0;
    virtual void SetToBegin() = 0;
    virtual std::string field_name(int n) = 0;
    virtual uint32 length(int n) = 0;

    virtual uint32 get_data(int n, char *p) = 0;
    virtual int32 get_int32(int n) = 0;
    virtual int64 get_int64(int n) = 0;
    virtual float get_float(int n) = 0;
    virtual double get_double(int n) = 0;
    virtual std::string get_string(int n) = 0;
};

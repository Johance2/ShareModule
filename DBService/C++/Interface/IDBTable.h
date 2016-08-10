#ifndef _IDBTable_H_
#define _IDBTable_H_

#include "IDBRecord.h"

class IDBTable
{
public:
    IDBRecord *FindRecord(int nID);
    IDBRecord *FindRecord(const char *pKey);
};

#endif
#ifndef _IDBTable_H_
#define _IDBTable_H_

#include "IDBRecord.h"

class IDBTable
{
public:
    virtual IDBRecord *FindRecord(int nID) = 0;
    virtual IDBRecord *FindRecord(const char *pKey) = 0;
	virtual IDBRecord *FindRecordByIndex(int nIndex) = 0;
	virtual size_t GetRecountCount() = 0;
};

#endif
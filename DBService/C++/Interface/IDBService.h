#ifndef __IDBService_H__
#define __IDBService_H__

#include "IDBTable.h"
#include "IKernel.h"

#define DBServiceModuleID 2

typedef const char* (*GetFileDataFun)(const char *pFileName);

#define CSVFILE_DIR		"CSV"

enum TableType
{
	TT_ModelInfo,	
	TT_Amount
};

class IDBService : public IModule
{
public:
	virtual void Init() = 0;
	virtual void Shut() = 0;
	virtual void Update(float fTime) = 0;
	virtual UCHAR GetModuleID() {return DBServiceModuleID;};

public:
    virtual IDBTable *FindTable(int nType) = 0;
    virtual IDBRecord *FindRecord(int nType, int nID) = 0;
    virtual IDBRecord *FindRecord(int nType, const char *pKey) = 0;
	virtual void SetGetFileDataFun(GetFileDataFun fun) = 0;
};

#endif // __IDBService_H__
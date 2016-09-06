#ifndef __IDBService_H__
#define __IDBService_H__

#include "IDBTable.h"
#include "IKernel.h"
#include "automake/DBDefine.h"

#define DBServiceModuleID 2

typedef const char* (*GetFileDataFun)(const char *pFileName);

#define CSVFILE_DIR		"CSV"

class IDBService : public IModule
{
public:
	virtual void Init(ModuleAttribute *pAttribute) = 0;
	virtual void Shut() = 0;
	virtual void Update(float fTime) = 0;
	virtual UCHAR GetModuleID() {return DBServiceModuleID;};

public:
    virtual IDBTable *FindTable(int nType) = 0;
    virtual IDBRecord *FindRecord(int nType, int nID) = 0;
    virtual IDBRecord *FindRecord(int nType, const char *pKey) = 0;
    virtual IDBBlock *FindData(int nType, int nID) = 0;
    virtual IDBBlock *FindData(int nType, const char *pKey) = 0;
	virtual void SetGetFileDataFun(GetFileDataFun fun) = 0;
};

extern IDBService *g_pDBService;

#endif // __IDBService_H__
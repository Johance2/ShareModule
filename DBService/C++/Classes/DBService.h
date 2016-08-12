#ifndef __DBService_H__
#define __DBService_H__

#include "../Interface/IDBService.h"
#include "DBTable.h"

class DBService : public IDBService
{
public:
    DBService();
    ~DBService();
	
	virtual void Init(ModuleAttribute *pAttribute);
	virtual void Shut();
	virtual void Update(float fTime);

    virtual DBTable *FindTable(int nType);
    virtual DBRecord *FindRecord(int nType, int nID);
    virtual DBRecord *FindRecord(int nType, const char *pKey);
    virtual IDBBlock *FindData(int nType, int nID);
    virtual IDBBlock *FindData(int nType, const char *pKey);
	
public:
	
private:
    std::vector<DBTable*> m_vecTables;

public:
	void SetGetFileDataFun(GetFileDataFun fun);

private:
	GetFileDataFun m_GetFileDataFun;
};

#endif // __DBService_H__
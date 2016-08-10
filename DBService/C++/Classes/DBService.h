#ifndef __DBService_H__
#define __DBService_H__

#include "../Interface/IDBService.h"
#include "DBTable.h"

class DBService : public IDBService
{
public:
    DBService(IKernel *pKernel);
    ~DBService();
	
	virtual void Init();
	virtual void Shut();
	virtual void Update(float fTime);

    DBTable *FindTable(int nType);
    DBRecord *FindRecord(int nType, int nID);
    DBRecord *FindRecord(int nType, const char *pKey);
	
public:
	
private:
    std::vector<DBTable*> m_vecTables;

public:
	void SetGetFileDataFun(GetFileDataFun fun);

private:
	GetFileDataFun m_GetFileDataFun;

protected:
	IKernel *m_pKernel;
};

#define g_pDBService ((DBService*)GameEnv::GetGameModule(eGMIT_DBService))

#endif // __DBService_H__
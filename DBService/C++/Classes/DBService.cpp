#include "DBService.h"
#include <assert.h>

#define LoadTable(name, key) \
    pTable = new DBTable(); \
	if(m_GetFileDataFun)\
	{\
		strData = m_GetFileDataFun((CSVFILE_DIR + std::string(#name) + ".csv").c_str()); \
	}\
	if(m_pKernel)\
	{\
		IDataStream *pStream = m_pKernel->GetFileData("");\
		strData = pStream->getAsString().c_str();\
		pStream->Release();\
	}\
	if (pTable->LoadFromMemory(strData.c_str()))\
	{\
		pTable->MakeKey(key);\
		m_vecTables[TT_##name] = pTable; \
	}\
	else\
	{\
		assert(0);\
		delete pTable;\
	}\


DBService::DBService(IKernel *pKernel)
{
	m_pKernel = pKernel;
	m_GetFileDataFun = NULL;
}

DBService::~DBService()
{
}

void DBService::Init()
{
    m_vecTables.resize(TT_Amount, NULL);
    DBTable *pTable = NULL;
    std::string strData;
    LoadTable(ModelInfo, "ID");
		
}

void DBService::Shut()
{

}

void DBService::Update(float fTime)
{
}

DBTable *DBService::FindTable(int nType)
{
    return m_vecTables[nType];
}

DBRecord *DBService::FindRecord(int nType, int nID)
{
    return m_vecTables[nType]->FindRecord(nID);
}

DBRecord *DBService::FindRecord(int nType, const char *pKey)
{
    return m_vecTables[nType]->FindRecord(*pKey);
}

void DBService::SetGetFileDataFun(GetFileDataFun fun)
{
	m_GetFileDataFun = fun;
}
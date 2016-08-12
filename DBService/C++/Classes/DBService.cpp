#include "DBService.h"
#include <assert.h>
#include "ILog.h"

DBService::DBService()
{
	m_GetFileDataFun = NULL;
}

DBService::~DBService()
{
}

void DBService::Init(ModuleAttribute *pAttribute)
{
	std::string strDBConfig;
	std::string strCSVDir;
	if(pAttribute)
	{
		strCSVDir = (*pAttribute)["csvDir"];
		strDBConfig = strCSVDir  + (*pAttribute)["config"];
	}

	std::vector<std::string> vecNames;
	int nTableCount = 0;
	if(g_pKernel)
	{
		IDataStream *pStream = g_pKernel->GetFileData(strDBConfig.c_str());
		if(pStream) 
		{
			pStream->read(&nTableCount, sizeof(nTableCount));
			vecNames.resize(nTableCount);
			for(int i = 0; i < nTableCount; i++)
			{
				char cLength;
				char szName[255];
				pStream->read(&cLength, sizeof(cLength));
				pStream->read(szName, cLength);
				szName[cLength] = 0;
				vecNames[i] = szName;
			}
			pStream->Release();
		}
	}
	
    m_vecTables.resize(nTableCount, NULL);
	for(int i = 0; i < nTableCount; i++)
	{
		DBTable *pTable = new DBTable(); 
		std::string strData ;
		if(g_pKernel)
		{
			IDataStream *pStream = g_pKernel->GetFileData((strCSVDir + vecNames[i] + ".csv").c_str());
			if(pStream) 
			{
				strData = pStream->getAsString().c_str();
				pStream->Release();
			}
		}
		if (pTable->LoadFromMemory(strData.c_str()))
		{
			pTable->MakeKey(0);
			m_vecTables[i] = pTable; 
		}
		else
		{
			assert(0);
			delete pTable;
		}
	}
	
	LOGFMTI("NetService Load Table:%d", nTableCount);
	
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

IDBBlock *DBService::FindData(int nType, int nID)
{
	IDBRecord *pRecord = FindRecord(nType, nID);
	if(pRecord)
	{
		return pRecord->Block();
	}
	return NULL;
}

IDBBlock *DBService::FindData(int nType, const char *pKey)
{
	IDBRecord *pRecord = FindRecord(nType, pKey);
	if(pRecord)
	{
		return pRecord->Block();
	}
	return NULL;
}

void DBService::SetGetFileDataFun(GetFileDataFun fun)
{
	m_GetFileDataFun = fun;
}
#include "LogicManager.h"
#include <assert.h>
#include "ILog.h"
#include "IDBService.h"

IDBService *g_pDBService;

void Utility_ConvertUtf8ToGBK(std::string& strUtf8) 
{
    int len=MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL,0);
    unsigned short * wszGBK = new unsigned short[len+1];        
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCCH)strUtf8.c_str(), -1, (LPWSTR)wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK=new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL,NULL);

    strUtf8 = szGBK;
    delete[] szGBK;
    delete[] wszGBK;
}

LogicManager::LogicManager()
{
}

LogicManager::~LogicManager()
{
}

void LogicManager::Init(ModuleAttribute *pAttribute)
{	
	g_pDBService = (IDBService*)g_pKernel->FindModule(DBServiceModuleID);

	IDBTable *pTable = g_pDBService->FindTable(TT_Example);
	{
		for(int i = 0; i < pTable->GetRecountCount(); i++)
		{
			std::string strContent = "";
			IDBRecord *pRecord =  pTable->FindRecordByIndex(i);
			if(pRecord)
			{
				strContent += pRecord->Attribute("ID");
				strContent += "\t";
				strContent += pRecord->Attribute("strValue");
				strContent += "\t";
				strContent += pRecord->Attribute("IntValue");
				strContent += "\t";
				strContent += pRecord->Attribute("FloatValue");
				strContent += "\n";
			}
			Utility_ConvertUtf8ToGBK(strContent);
			LOGI(strContent.c_str());
		}
	}
	{
		for(int i = 0; i < pTable->GetRecountCount(); i++)
		{
			std::string strContent = "";
			DBData_Example *pData =  (DBData_Example*)pTable->FindDataByIndex(i);
			if(pData)
			{
				char szTemp[1024];
				sprintf(szTemp, "%d\t%s\t%d\t%f\t", pData->ID, pData->strValue, pData->IntValue, pData->FloatValue);
				strContent += szTemp;
				strContent += "\n";
			}
			Utility_ConvertUtf8ToGBK(strContent);
			LOGI(strContent.c_str());
		}
	}
}

void LogicManager::Shut()
{

}

void LogicManager::Update(float fTime)
{
}
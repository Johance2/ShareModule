#include "LogicManager.h"
#include <assert.h>
#include "ILog.h"
#include "IDBService.h"

IDBService *g_pDBService;

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
			g_pKernel->ConvertUtf8ToGBK(strContent);
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
			g_pKernel->ConvertUtf8ToGBK(strContent);
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
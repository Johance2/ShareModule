#include "Kernel.h"
#include "tinyxml\tinyxml.h"
#include "DataStream.h"
#include "log4z.h"
using namespace zsummer::log4z;  

typedef IModule* (*CreateModule)();

Kernel::Kernel(const char *pConfig)
{
	m_strConfig = pConfig;
	m_bWillQuit = false;
}

Kernel::~Kernel()
{
}

int  Kernel::Run()
{
	if(!Init())
		return 0;

	while(1)
	{
		if(m_bWillQuit)
			break;

		Update();
	}

	Shut();

	return 0;
}

bool Kernel::Init()
{
	//start log4z  
	ILog4zManager::getRef().start();
	
	// 预创建 255个 槽位
	m_vecModuleHandle.resize(255, 0);
	m_vecModule.resize(255, 0);

	TiXmlDocument doc;

	IDataStream *pStream = GetFileData(m_strConfig.c_str());	
	if(!pStream)
	{
		LOGFMTE("config file not found:%s", m_strConfig.c_str());
		return false;
	}
	doc.Parse(pStream->getAsString().c_str());
	pStream->Release();
	if(doc.Error())
	{
		LOGFMTE("config load failure:%s err:%s", m_strConfig.c_str(), doc.ErrorDesc());
		return false;
	}

	TiXmlElement *pRoot = doc.RootElement();
	if(!pRoot)
	{
		LOGFMTE("config is empty:%s", m_strConfig.c_str());
	}

	TiXmlElement *pChild = pRoot->FirstChildElement();
	while(pChild)
	{
		AddModuleByName(pChild->Attribute("Name"));
		pChild = pChild->NextSiblingElement();
	}

	return true;
}

void Kernel::Shut()
{
	ILog4zManager::getRef().stop();  
}

void Kernel::Update()
{
	static INT64 nLastTime = GetCuttentTime();
	INT64 nCurrentTime = GetCuttentTime();
	INT64 nElapseTime = nCurrentTime - nLastTime;	
	UpdateModule(nElapseTime / 1000.0f);
}

bool Kernel::AddModule(IModule *pModule)
{
	AddModuleHandle(pModule, NULL);
	return true;
}

bool Kernel::AddModuleHandle(IModule *pModule, MODULE_HANDLE handle)
{
	UCHAR ucID = pModule->GetModuleID();
	if(m_vecModule[ucID] != NULL || (handle != NULL && m_vecModuleHandle[ucID] != NULL))
	{
		LOGFMTE("has same module id:%d", (int)ucID);
		return false;
	}

	m_vecModule[ucID] = pModule;
	m_vecModuleHandle[ucID] = handle;

	m_listModuleID.push_back(ucID);

	return true;
}

void Kernel::RemoveModule(IModule *pModule)
{
}

void Kernel::UpdateModule(float fTime)
{
	std::list<UCHAR>::iterator it = m_listModuleID.begin();
	for(;it != m_listModuleID.end(); ++it)
	{
		m_vecModule[*it]->Update(fTime);
	}
}

bool Kernel::AddModuleByName(const char *moduleName)
{
#ifdef WIN32
	std::string strDllName = moduleName;
	std::string strName = moduleName;
	int nIndex = strDllName.find_last_of("\\/");
	if(nIndex != -1)
	{
		std::string strDllDir = strDllName.substr(0, nIndex);
		SetDllDirectoryA(strDllDir.c_str());
		strName = strDllName.substr(nIndex+1);
	}
#ifdef DEBUG
	strDllName += "_d";
#endif
	strDllName += ".dll";

	HMODULE hModule = LoadLibraryA(strDllName.c_str());
	char szDllPath[_MAX_PATH];
	GetCurrentDirectoryA(_MAX_PATH, szDllPath);
	SetDllDirectoryA(szDllPath);
	if(!hModule)
	{
		LOGFMTE("Module not found:%s", strName.c_str());
		return false;
	}
	
	CreateModule proc = (CreateModule)GetProcAddress(hModule, "CreateModule");
	if(!proc)
	{
		LOGFMTE("CreateModule symbol not found int module:%s", strName.c_str());
		return false;
	}
	IModule *pModue = proc();
	if(!pModue)
	{
		LOGFMTE("CreateModule failure:%s", strName.c_str());
		return false;
	}
#else
#endif
	if(AddModuleHandle(pModue, hModule))
	{
		LOGFMTI("LoadModule: ID:%d\t Name:%s\t\t\t", (int)pModue->GetModuleID(), strName.c_str());
		return true;
	}

	return false;
}

IModule* Kernel::FindModule(UCHAR ucID)
{
	return m_vecModule[ucID];
}

INT64 Kernel::GetCuttentTime()
{
#ifdef WIN32
            typedef union {
                unsigned __int64 ft_scalar;
                FILETIME ft_struct;
            } FT;
            // Number of 100 nanosecond units from 1/1/1601 to 1/1/1970
#define EPOCH_BIAS  116444736000000000i64

            // 获得64位的当前时间,精确到ms
                INT64 tim;
                FT nt_time;
                GetSystemTimeAsFileTime( &(nt_time.ft_struct) );
                tim = (INT64)((nt_time.ft_scalar - EPOCH_BIAS) / 10000);

                return tim;
#elif defined linux
            struct timeval tv;
            gettimeofday(&tv, NULL);
            return tv.tv_sec * 1000 + tv.tv_usec/1000;
#endif //linux

}

IDataStream* Kernel::GetFileData(const char *pFileName)
{
	FILE *fp = fopen(pFileName, "rb");
	if(!fp)
	{
		return NULL;
	}

	return new FileHandleDataStream(fp, DataStream::READ);
}
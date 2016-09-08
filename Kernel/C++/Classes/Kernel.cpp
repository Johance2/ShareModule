#include "Kernel.h"
#include "tinyxml\tinyxml.h"
#include "DataStream.h"
#include "log4z.h"
#include <stdarg.h>
using namespace zsummer::log4z;  

typedef IModule* (*CreateModule)(IKernel *pKernel);

Kernel::Kernel()
{
	m_bWillQuit = false;
	m_bHasConsole = true;
}

Kernel::~Kernel()
{
}

int  Kernel::Run(const char *pConfig)
{
	m_strConfig = pConfig;
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
	m_vecModuleAttribute.resize(255, 0);

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
	if(pRoot->Attribute("hasConsole"))
	{
		m_bHasConsole = strcmp("true", pRoot->Attribute("hasConsole")) == 0;
	}
	if(m_bHasConsole)
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}

	TiXmlElement *pChild = pRoot->FirstChildElement();
	while(pChild)
	{
		IModule *pModue = AddModuleByName(pChild->Attribute("Name"));
		if(pModue)
		{
			ModuleAttribute *pModuleAttr = new ModuleAttribute();
			TiXmlAttribute*pAttr = pChild->FirstAttribute();
			while(pAttr)
			{
				(*pModuleAttr)[pAttr->Name()] = pAttr->Value() ? pAttr->Value() : "";
				pAttr = pAttr->Next();
			}
			m_vecModuleAttribute[pModue->GetModuleID()]  = pModuleAttr;
		}
		pChild = pChild->NextSiblingElement();
	}
	
	std::list<UCHAR>::iterator it = m_listModuleID.begin();
	for(;it != m_listModuleID.end(); ++it)
	{
		m_vecModule[*it]->Init(m_vecModuleAttribute[*it]);
	}
	return true;
}

void Kernel::Shut()
{
	std::list<UCHAR>::iterator it = m_listModuleID.begin();
	for(;it != m_listModuleID.end(); ++it)
	{
		m_vecModule[*it]->Shut();
		delete m_vecModuleAttribute[*it];
	}
	ILog4zManager::getRef().stop();  
	if(m_bHasConsole)
		FreeConsole();
}

void Kernel::Update()
{
	static INT64 nLastTime = GetCuttentTime();
	INT64 nCurrentTime = GetCuttentTime();
	INT64 nElapseTime = nCurrentTime - nLastTime;	
	UpdateModule(nElapseTime / 1000.0f);
}

void Kernel::Quit()
{
	m_bWillQuit = true;
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

IModule* Kernel::AddModuleByName(const char *moduleName)
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
		return NULL;
	}
	
	CreateModule proc = (CreateModule)GetProcAddress(hModule, "CreateModule");
	if(!proc)
	{
		LOGFMTE("CreateModule symbol not found int module:%s", strName.c_str());
		return NULL;
	}
	IModule *pModue = proc(this);
	if(!pModue)
	{
		LOGFMTE("CreateModule failure:%s", strName.c_str());
		return NULL;
	}
#else
#endif
	if(AddModuleHandle(pModue, hModule))
	{
		LOGFMTI("LoadModule: ID:%d\t Name:%s\t\t\t", (int)pModue->GetModuleID(), strName.c_str());
		return pModue;
	}

	return NULL;
}

IModule* Kernel::FindModule(UCHAR ucID)
{
	return m_vecModule[ucID];
}

MODULE_HANDLE Kernel::FindModuleHandle(UCHAR ucID)
{
	return m_vecModuleHandle[ucID];
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
	FILE *fp;
	if(fopen_s(&fp, pFileName, "rb") != 0)
	{
		return NULL;
	}

	return new FileHandleDataStream(fp, DataStream::READ);
}

void Kernel::ConvertUtf8ToGBK(std::string& strUtf8)
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

void Kernel::ConvertGBKToUtf8(std::string& strGBK)
{
    int len=MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL,0);
    unsigned short * wszUtf8 = new unsigned short[len+1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCCH)strGBK.c_str(), -1, (LPWSTR)wszUtf8, len);        

    len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
    char *szUtf8=new char[len + 1];
    memset(szUtf8, 0, len + 1);        
    WideCharToMultiByte (CP_UTF8, 0, (LPCWCH)wszUtf8, -1, szUtf8, len, NULL,NULL);

    strGBK = szUtf8;
    delete[] szUtf8;                
    delete[] wszUtf8;   
}

std::wstring Kernel::ConvertUtf8ToWideChar(const std::string& strUtf8)
{
    std::wstring ret;
    if (!strUtf8.empty())
    {
        int nNum = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, nullptr, 0);
        if (nNum)
        {
            WCHAR* wideCharString = new WCHAR[nNum + 1];
            wideCharString[0] = 0;

            nNum = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, wideCharString, nNum + 1);

            ret = wideCharString;
            delete[] wideCharString;
        }
    }
    return ret;
}

std::string Kernel::ConvertWideCharToUtf8(const std::wstring& strWideChar)
{
    std::string ret;
    if (!strWideChar.empty())
    {
        int nNum = WideCharToMultiByte(CP_UTF8, 0, strWideChar.c_str(), -1, nullptr, 0, nullptr, FALSE);
        if (nNum)
        {
            char* utf8String = new char[nNum + 1];
            utf8String[0] = 0;

            nNum = WideCharToMultiByte(CP_UTF8, 0, strWideChar.c_str(), -1, utf8String, nNum + 1, nullptr, FALSE);

            ret = utf8String;
            delete[] utf8String;
        }
    }

    return ret;
}

void Kernel::log_stream(int id, int level, const char *file, int line, const char *log)
{
	LOG_STREAM(id, level, file, line, log);
}

void Kernel::log_format(int id, int level, const char *file, int line, const char *logformat, ...)
{
	va_list ap; 
	va_start(ap, logformat); 
	char szBuffer[LOG4Z_LOG_BUF_SIZE];
	vsprintf_s(szBuffer, logformat, ap);
	va_end(ap);
	LOG_STREAM(id, level, file, line, szBuffer);
}
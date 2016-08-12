#ifndef _Kernel_H_
#define _Kernel_H_
#include "..\Interface\IKernel.h"
#include <vector>
#include <list>

#ifdef _WIN32
#include <windows.h>
typedef HMODULE MODULE_HANDLE;
#else
#endif

class Kernel : public IKernel
{
public:
	Kernel(const char *pConfig);
	~Kernel();

	int Run();

protected:
	virtual bool Init();
	virtual void Shut();
	virtual void Update();

public:
    virtual bool AddModule(IModule *pModule);
    virtual bool AddModuleHandle(IModule *pModule, MODULE_HANDLE handle);
    virtual void RemoveModule(IModule *pModule);
	virtual void UpdateModule(float fTime);

protected:	
    virtual IModule* AddModuleByName(const char *moduleName);

private:
	std::string m_strConfig;
	bool m_bWillQuit;


private:
	std::vector<MODULE_HANDLE> m_vecModuleHandle;
	std::vector<IModule*> m_vecModule;
	std::vector<ModuleAttribute*> m_vecModuleAttribute;
	std::list<UCHAR> m_listModuleID;
	
public:
	// ½Ó¿Ú
	virtual IModule* FindModule(UCHAR ucID);
	virtual INT64 GetCuttentTime();
	virtual IDataStream *GetFileData(const char *pFileName);
	virtual void log_stream(int id, int level, const char *file, int line, const char *log);
	virtual void log_format(int id, int level, const char *file, int line, const char *logformat, ...);
};

#endif
#include "SQLiteService.h"
#define ModuleAPI extern "C" __declspec(dllexport)

ModuleAPI IModule *CreateModule(IKernel *pKernel)
{
	return new SQLiteService(pKernel);
}

ModuleAPI void DestroyModule(IModule *pModule)
{
	delete pModule;
}

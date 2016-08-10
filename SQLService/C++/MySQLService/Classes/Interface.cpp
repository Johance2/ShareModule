#include "MySQLService.h"
#define ModuleAPI extern "C" __declspec(dllexport)

ModuleAPI IModule *CreateModule(IKernel *pKernel)
{
	return new MySQLService(pKernel);
}

ModuleAPI void DestroyModule(IModule *pModule)
{
	delete pModule;
}

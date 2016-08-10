#include "ODBCService.h"
#define ModuleAPI extern "C" __declspec(dllexport)

ModuleAPI IModule *CreateModule(IKernel *pKernel)
{
	return new ODBCService(pKernel);
}

ModuleAPI void DestroyModule(IModule *pModule)
{
	delete pModule;
}

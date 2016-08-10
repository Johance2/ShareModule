#include "DBService.h"
#define ModuleAPI extern "C" __declspec(dllexport)
#include "DBService.h"

ModuleAPI IModule *CreateModule(IKernel *pKernel)
{
	return new DBService(pKernel);
}

ModuleAPI void DestroyModule(IModule *pModule)
{
	delete pModule;
}

#include "NetService.h"
#define ModuleAPI extern "C" __declspec(dllexport)
#include "NetService.h"

ModuleAPI IModule *CreateModule(IKernel *pKernel)
{
	return new NetService(pKernel);
}

ModuleAPI void DestroyModule(IModule *pModule)
{
	delete pModule;
}

#include "DBService.h"
#define ModuleAPI extern "C" __declspec(dllexport)
#include "DBService.h"

ModuleAPI IDBService *CreateModule()
{
	return new DBService();
}

ModuleAPI void DestroyModule(IDBService *pModule)
{
	delete pModule;
}

#include "..\Classes\Kernel.h"

//int main(int argc, char **argvs)
//{
//	if(argc > 1)
//	{
//		Kernel kernel(argvs[1]);
//		kernel.Run();
//	}
//	return 0;
//}

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include "MiniDump.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CMiniDump dump;
    LPWSTR *szArgList;
    int argCount;
    szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	
	Kernel *pKernel = new Kernel();
    if (szArgList != NULL && argCount > 1)
	{
		std::string configFile = pKernel->ConvertWideCharToUtf8(szArgList[1]);
		pKernel->Run(configFile.c_str());
	}
	delete pKernel;
	return 0;
}
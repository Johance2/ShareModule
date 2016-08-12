// DatasetTool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "CommandBase.h"
#include "Dataset/DatasetCommand.h"

int _tmain(int argc, _TCHAR* argv[])
{
    CommandBase::AddCommand("dataset", new DatasetCommand());

	if(argc > 1)
	{
		CommandBase *pCmd = CommandBase::GetCommand(argv[1]);
		if(pCmd)
		{
			pCmd->parseArgv(argc, argv);
			if(!pCmd->run())
			{
                cout << pCmd->desc() << endl;
                system("pause");
			}
		}
	}
	else
	{
		cout << CommandBase::GetHelp() << endl;
		system("pause");
	}
	return 0;
}


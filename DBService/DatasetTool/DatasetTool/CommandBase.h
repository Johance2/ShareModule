#pragma once

#include "Utility.h" 

class CommandBase
{
public:
	CommandBase(void);
	~CommandBase(void);

	static CommandBase *GetCommand(const char *pName);
	static bool AddCommand(const char *pName, CommandBase *pCmd);
	static const char* GetHelp();
	virtual const char *desc() = 0;
	virtual void parseArgv(int nVar, _TCHAR* argv[]) = 0;
	virtual bool run() = 0;

private:
	typedef std::map<std::string, CommandBase*> CMDMap;
	static CMDMap m_mapCMD;
	static std::string m_strHelp;
};
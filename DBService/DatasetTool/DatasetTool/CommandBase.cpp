#include "StdAfx.h"
#include "CommandBase.h"

CommandBase::CMDMap CommandBase::m_mapCMD;
std::string CommandBase::m_strHelp = "";

CommandBase::CommandBase(void)
{
}


CommandBase::~CommandBase(void)
{
}

CommandBase *CommandBase::GetCommand(const char *pName)
{
	CommandBase *pCmd = NULL;
	CMDMap::iterator it = m_mapCMD.find(pName);
	if(it != m_mapCMD.end())
	{
		pCmd = it->second;
	}

	return pCmd;
}

bool CommandBase::AddCommand(const char *pName, CommandBase *pCmd)
{
	if(GetCommand(pName))
		return false;

	m_mapCMD[pName] = pCmd;

	return true;
}

const char*  CommandBase::GetHelp()
{
	if(m_strHelp.size() == 0)
	{
		CMDMap::iterator it = m_mapCMD.begin();
		while(it != m_mapCMD.end())
		{
			m_strHelp += it->second->desc();
			++it;
		}
	}

	return m_strHelp.c_str();
}
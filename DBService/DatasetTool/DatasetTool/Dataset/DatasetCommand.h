#pragma once

#include "commandbase.h"

class DatasetCommand :
	public CommandBase
{
public:
	DatasetCommand(void);
	~DatasetCommand(void);
	
	virtual const char *desc();
	virtual void parseArgv(int nVar, _TCHAR* argv[]);
	virtual bool run();

private:
	bool ExportPack();
	bool ParseXlsx(const char *pPath);
	bool CacleInfo(const char *pDir);

private:
	std::string m_strFromDir;
	std::string m_strCodeDir;
	std::string m_strCSVDir;
	int m_nTag;

private:
	struct DBData
	{
		std::string strName;
		std::string strType;
		std::string strDesc;
	};
	typedef std::vector<DBData>	VecProperty;
	typedef std::map<std::string, VecProperty> MapTable;

	MapTable m_mapTable;
};


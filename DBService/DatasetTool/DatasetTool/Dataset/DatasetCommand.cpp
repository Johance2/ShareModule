#include "StdAfx.h"
#include "DatasetCommand.h"
#include "MiniExcelReader.h"

#define NAME_ROW (1)
#define TYPE_ROW (2)
#define DESC_ROW (3)
#define TAG_ROW (4)
#define CONTENT_ROW (5)

DatasetCommand::DatasetCommand(void)
{
}


DatasetCommand::~DatasetCommand(void)
{
}

const char *DatasetCommand::desc()
{
	return	"-----------------------------------Dataset-------------------------------------\r\n"
			"Dataset \r\n"
			"flag: -xlsx [dir,dir] \r\n"
           "flag: -code [codeDir] \r\n"
           "flag: -csv [csvdir] \r\n";
}

void DatasetCommand::parseArgv(int nVar, _TCHAR* argv[])
{
	for(int i = 2; i < nVar; i++)
	{
		std::string strVal = argv[i];
		if(strVal == "-xlsx")
		{
			m_strFromDir = argv[++i];
        }
		else if(strVal == "-code")
		{
			m_strCodeDir = argv[++i];
        }
		else if(strVal == "-csv")
		{
			m_strCSVDir = argv[++i];
        }
		else if(strVal == "-tag")
		{
			m_nTag = atoi(argv[++i]);
        }
	}
}

bool DatasetCommand::run()
{
	VecString vecXlsxDir;
	Utility_split(m_strFromDir.c_str(), ",", vecXlsxDir);
	
	// 获取所有需要导出的Xlsx
	VecString files;
	for(auto i = 0; i < vecXlsxDir.size(); i++)
	{
		Utility_Find(vecXlsxDir[i].c_str(), files, true, "*.xlsx");
	}


	for(auto i = 0; i < files.size(); i++)
	{
		ParseXlsx(files[i].c_str());
	}

	
		// 导出代码
		if(m_strCodeDir.size() > 0)
		{
			CreatDir((char*)m_strCodeDir.c_str());

			std::string strDBDefine;

			strDBDefine = ""
				"#ifndef _DBDefine_Automake_H_\r\n"
				"#define _DBDefine_Automake_H_\r\n";

			strDBDefine += "\r\nstruct IDBBlock{};\r\n";

			strDBDefine += "enum TableType\r\n{\r\n";
			for(auto itrProps = m_mapTable.begin(); itrProps != m_mapTable.end(); ++itrProps)
			{
				auto vecPropertys = itrProps->second;
				if(vecPropertys.size() == 0)
					continue;

				strDBDefine += "\tTT_";
				strDBDefine += itrProps->first;
				strDBDefine += ",\r\n";
			}

			strDBDefine += "\tTT_Amount";
			strDBDefine += "\r\n};\r\n\r\n";
			for(auto itrProps = m_mapTable.begin(); itrProps != m_mapTable.end(); ++itrProps)
			{
				auto vecPropertys = itrProps->second;
				if(vecPropertys.size() == 0)
					continue;

				strDBDefine += "struct DBData_";
				strDBDefine += itrProps->first;
				strDBDefine += " : IDBBlock";
				
				strDBDefine += "\r\n{\r\n";
				
				for(auto i = 0; i < vecPropertys.size(); i++)
				{
					strDBDefine += "\t";					
					if(vecPropertys[i].strType == "string")
					{
						strDBDefine += "const char*";		
					}
					else
					{
						strDBDefine += vecPropertys[i].strType;					
					}
					strDBDefine += " ";
					strDBDefine += vecPropertys[i].strName;
					strDBDefine += ";//";
					strDBDefine += vecPropertys[i].strDesc;		
					if(i+1 != vecPropertys.size())
						strDBDefine += "\r\n";
				}
				
				strDBDefine += "\r\n};\r\n\r\n";

			}


			strDBDefine += ""
				"#endif";

			Utility_ConvertUtf8ToGBK(strDBDefine);
			FILE *fp = fopen((m_strCodeDir+"DBDefine.h").c_str(), "wb");

			fwrite(strDBDefine.c_str(), strDBDefine.size(), 1, fp);

			fclose(fp);
		}
		
		// 导出csv的表名数据
		if(m_strCSVDir.size() > 0)
		{
			FILE *fp = fopen((m_strCSVDir+"_index").c_str(), "wb");

			int nCount = m_mapTable.size();
			fwrite(&nCount, sizeof(nCount), 1, fp);
			
			for(auto itrProps = m_mapTable.begin(); itrProps != m_mapTable.end(); ++itrProps)
			{
				char cLength = itrProps->first.size();
				fwrite(&cLength, sizeof(cLength), 1, fp);
				fwrite(itrProps->first.c_str(), cLength, 1, fp);
			}

			fclose(fp);
		}

	return true;
}

bool DatasetCommand::ParseXlsx(const char *pPath)
{
	MiniExcelReader::ExcelFile excelFile;

	if(!excelFile.open(pPath))
		return false;

	auto &sheets = excelFile.sheets();
	
	std::string strXlsxName = GetFileName(pPath, false);

	for(auto i = 0; i < sheets.size(); i++)
	{
		auto &sheet = sheets[i];
		// 索引不是从 0  开始的 需要注意一下
		auto &range = sheet.getDimension();

		std::string sheetName = strXlsxName;
		if(sheets.size() > 1)
		{
			sheetName.append("_");
			sheetName.append(sheet.getName());
		}
		std::cout << sheetName << std::endl;
		
		// 获取导出标记
		std::vector<bool> vecExportFlag;
		vecExportFlag.resize(sheet.getDimension().lastCol-1);
		for(int col = 2; col <= range.lastCol; col++)
		{
			auto cell = sheet.getCell(4, col);
			int nTag = atoi(cell->value.c_str());
			vecExportFlag[col-2] = nTag & m_nTag;
		}

		auto &vecProperty = m_mapTable[sheetName];

		std::string strHeader;
		std::string strType;
		for(int col = 2; col <= range.lastCol; col++)
		{
			if(vecExportFlag[col-2])
			{
				auto cell1 = sheet.getCell(NAME_ROW, col);
				strHeader.append(cell1->value);
				strHeader.append(",");

				auto cell2 = sheet.getCell(TYPE_ROW, col);
				strType.append(cell2->value);
				strType.append(",");

				auto cell3 = sheet.getCell(DESC_ROW, col);
				DBData data;
				data.strName = cell1->value;
				data.strType = cell2->value;
				data.strDesc = cell3->value;
				vecProperty.push_back(data);
			}
		}

		// 导出csv
		if(m_strCSVDir.size() > 0)
		{
			strHeader.pop_back();
			strType.pop_back();
			strHeader.append("\n");
			strType.append("\n");

			std::string strContent;
			for(int row = CONTENT_ROW; row <= range.lastRow; row++)
			{
				for(int col = 2; col <= range.lastCol; col++)
				{
					auto cell = sheet.getCell(row, col);
					if(vecExportFlag[col-2])
					{
						std::string strValue = cell->value;
						Utility_ReplaceString(strValue, "\"", "\"\"");
						if(strValue.find_first_of(',') != -1)
						{
							strValue.insert(0, "\"");
							strValue.push_back('\"');
						}
						strContent.append(strValue);
						strContent.append(",");
					}
				}
				strContent.pop_back();
				strContent.append("\n");
			}

			CreatDir((char*)m_strCSVDir.c_str());

			FILE *fp = fopen((m_strCSVDir+sheetName+".csv").c_str(), "wb");

			fwrite(strHeader.c_str(), strHeader.size(), 1, fp);
			fwrite(strType.c_str(), strType.size(), 1, fp);
			fwrite(strContent.c_str(), strContent.size(), 1, fp);

			fclose(fp);
		}
	}

	return true;
}

bool DatasetCommand::ExportPack()
{

	return true;
}

bool DatasetCommand::CacleInfo(const char *pDir)
{
	char szFullParentPath[_MAX_PATH];
	char szFullPath[_MAX_PATH];
	string strPath = m_strFromDir;
	_fullpath(szFullPath, strPath.c_str(), _MAX_PATH);
	strPath += "/../";
	_fullpath(szFullParentPath, strPath.c_str(), _MAX_PATH);
	int nDirLength = strlen(szFullParentPath);
	nDirLength = strlen(szFullPath);
	return true;
}

#include "StdAfx.h"
#include "Utility.h"
#ifdef WIN32  

#include <direct.h>  
#include <io.h>  

#elif LINUX  

#include <stdarg.h>  
#include <sys/stat.h>  

#endif  

#ifdef WIN32  

#define ACCESS _access  
#define MKDIR(a) _mkdir((a))  

#elif LINUX  

#define ACCESS access  
#define MKDIR(a) mkdir((a),0755)  

#endif 

void Utility_ConvertUtf8ToGBK(std::string& strUtf8) 
{
    int len=MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL,0);
    unsigned short * wszGBK = new unsigned short[len+1];        
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCCH)strUtf8.c_str(), -1, (LPWSTR)wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK=new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL,NULL);

    strUtf8 = szGBK;
    delete[] szGBK;
    delete[] wszGBK;
}

void Utility_ConvertGBKToUtf8(std::string& strGBK) 
{
    int len=MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL,0);
    unsigned short * wszUtf8 = new unsigned short[len+1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCCH)strGBK.c_str(), -1, (LPWSTR)wszUtf8, len);        

    len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
    char *szUtf8=new char[len + 1];
    memset(szUtf8, 0, len + 1);        
    WideCharToMultiByte (CP_UTF8, 0, (LPCWCH)wszUtf8, -1, szUtf8, len, NULL,NULL);

    strGBK = szUtf8;
    delete[] szUtf8;                
    delete[] wszUtf8;                
}

bool Utility_ReplaceString(std::string& str, const std::string& searchString, const std::string& replaceString)
{
	std::string::size_type pos = 0;

	bool bRet = false;
	while ( (pos = str.find(searchString, pos)) != std::string::npos )
	{
		str.replace( pos, searchString.size(), replaceString);
		pos+=replaceString.size();

		bRet = true;
	}

	return bRet;
}

void split(std::string src, const char* token, VecString& vect)
{
    int nend = 0;
    int nbegin = 0;
    while (nend != -1)
    {
        nend = src.find(token, nbegin);
        if (nend == -1)
            vect.push_back(src.substr(nbegin, src.length() - nbegin));
        else
            vect.push_back(src.substr(nbegin, nend - nbegin));
        nbegin = nend + strlen(token);
    }
}

void Utility_split(std::string src, const char* token, VecString& vect)
{
    split(src, token, vect);
}

//只能处理目录:lpPath只能是路径
void Utility_Find(const char *lpPath, VecString &vecFiles, bool bRecus/* = true*/, const char *pExt/* = "*.*"*/)
{
    char szFind[MAX_PATH];
   char szFile[MAX_PATH];

    WIN32_FIND_DATAA FindFileData;

    strcpy(szFind,lpPath);
    strcat(szFind,"/");
    strcat(szFind, pExt);

    bool bAll = strcmp(pExt, "*.*") == 0;

    HANDLE hFind=::FindFirstFileA(szFind,&FindFileData);
    if(INVALID_HANDLE_VALUE == hFind)    return;
    while(TRUE)
    {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (FindFileData.cFileName[0] != '.'  && bRecus)
            {
                strcpy(szFile,lpPath);
                strcat(szFile,"/");
                strcat(szFile,FindFileData.cFileName);
                Utility_Find(szFile, vecFiles, bRecus, pExt);
            }
        }
        else
        {   
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
            vecFiles.push_back(std::string(lpPath)+"/"+FindFileData.cFileName);
            //cout<<FindFileData.cFileName<<endl;
        }
        if(!FindNextFileA(hFind,&FindFileData))
            break;
    }
    FindClose(hFind); 
}

void Utility_GetStringFromFile(const std::string &name, std::string &value)
{
    FILE *fp = fopen(name.c_str(), "rb");
    if (fp == NULL)
        return;

    long nSize = _filelength(_fileno(fp));
    value.resize(nSize);
    fread((char*)value.data(), nSize, 1, fp);
    fclose(fp);
}

unsigned char* Utility_GetDataFromFile(const std::string &name, long &nSize)
{
    FILE *fp = fopen(name.c_str(), "rb");
    if (fp == NULL)
        return 0;

    nSize = _filelength(_fileno(fp));
    unsigned char *pData = (unsigned char*)malloc(nSize);
    fread((unsigned char *)pData, nSize, 1, fp);
    fclose(fp);

    return pData;
}

void Utility_SaveStringToFile(const std::string& value, const std::string& name)
{
	FILE *fp = fopen(name.c_str(), "wb");
	if (fp == NULL)
		return;

	fwrite(value.c_str(), value.size(), 1, fp);
	fclose(fp);
}

void Utility_SaveDataToFile(void *data, long nSize, const std::string& name)
{
    FILE *fp = fopen(name.c_str(), "wb");
    if (fp == NULL)
        return;

    fwrite(data, nSize, 1, fp);
    fclose(fp);
}

void Utility_ReadNameInt(char **pName, void *pData, int *pPos)
{
    int nLength;
    int nPos = *pPos;
    Utility_ReadValue(&nLength, (char*)pData, pPos);
    nPos = *pPos;
    if(*pName == NULL)
    {
        *pName = new char[nLength+1];
    }
    strncpy(*pName, (char*)pData+nPos, nLength);
    (*pName)[nLength] = 0;
    *pPos = nPos + nLength;
}

void Utility_ReadNameChar(char **pName, void *pData, int *pPos)
{
    char cLength;
    int nPos = *pPos;
    Utility_ReadValue(&cLength, (char*)pData, pPos);
    nPos = *pPos;
    if(*pName == NULL)
    {
        *pName = new char[cLength+1];
    }
    strncpy(*pName, (char*)pData+nPos, cLength);
    (*pName)[cLength] = 0;
    *pPos = nPos + cLength;
}


void Utility_ReadData(void* data, void *pData, int nSize, int *pPos)
{
	memcpy(data, (char*)pData+*pPos, nSize);
    *pPos = *pPos + nSize;
}

void Utility_WriteNameInt(char *pName, FILE *fp)
{
    int nLength = strlen(pName);
    fwrite(&nLength, sizeof(int), 1, fp);
    fwrite(pName, nLength, 1, fp);
}

void Utility_CreateProcess(std::string strCommand)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi; 

    ZeroMemory( &si, sizeof(si) );

    si.cb = sizeof(si);

    ZeroMemory( &pi, sizeof(pi) ); 

    // Start the child process. 

    if( !CreateProcessA( NULL,   // No module name (use command line)

       (char*)strCommand.c_str(),      // Command line

       NULL,           // Process handle not inheritable

       NULL,           // Thread handle not inheritable

       FALSE,          // Set handle inheritance to FALSE

       0,              // No creation flags

       NULL,           // Use parent's environment block

       NULL,           // Use parent's starting directory 

       &si,            // Pointer to STARTUPINFO structure

       &pi )           // Pointer to PROCESS_INFORMATION structure

       ) 

    {

       printf( "CreateProcess failed (%d)./n", GetLastError() );

       return;

    } 

    // Wait until child process exits.

    WaitForSingleObject( pi.hProcess, INFINITE );
    // Close process and thread handles. 

    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

int CreatDir(char *pszDir)
{
    char szTemp[_MAX_PATH];
    strcpy(szTemp, pszDir);
	int i = 0;
	int iRet;
	int iLen = strlen(szTemp);

	//在末尾加/;
	if (szTemp[iLen - 1] != '\\' && szTemp[iLen - 1] != '/')
	{
		szTemp[iLen] = '/';
		szTemp[iLen + 1] = '\0';
	}

	// 创建目录;
	for (i = 0; i <= iLen; i++)
	{
		if (szTemp[i] == '\\' || szTemp[i] == '/')
		{
			szTemp[i] = '\0';

			//如果不存在,创建;  
			iRet = ACCESS(szTemp, 0);
			if (iRet != 0)
			{
				iRet = MKDIR(szTemp);
				if (iRet != 0)
				{
					return -1;
				}
			}
			//支持linux,将所有\换成/;  
			szTemp[i] = '/';
		}
	}

	return 0;
}

std::string GetFileDir(const std::string& pFile)
{
	std::string ret = pFile.substr(0, pFile.find_last_of("/")) + "/";
	return ret;
}
std::string GetFileName(const std::string& pFile, bool HasExt/* = true*/)
{
    std::string ret = pFile.substr(pFile.find_last_of("/")+1);
	if(!HasExt)
	{
		int nIndex = ret.find_last_of(".");
		if(nIndex != -1)
		{
			ret = ret.substr(0, nIndex);
		}
	}

    return ret;
}

const std::string& Utility_NormaliseDir(std::string& strDir)
{
	if (strDir.empty())
	{
		return strDir;
	}
	int nPos1 = strDir.rfind("/");
	int nPos2 = strDir.rfind("\\");

	if (std::string::npos == nPos1
		&& std::string::npos == nPos2)
	{
		strDir.append("/");
	}
	else if (std::string::npos != nPos1)
	{
		strDir.append("/");
	}
	else if (std::string::npos != nPos2)
	{
		strDir.append("\\");
	}
	
	return strDir;
}
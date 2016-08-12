#ifndef Utility__H_H_
#define Utility__H_H_

#include <windows.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <map>
#include <cmath>	
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <list>
#include <math.h>
using namespace std;


typedef std::vector<string> VecString;

void Utility_ConvertUtf8ToGBK(std::string& strUtf8);
void Utility_ConvertGBKToUtf8(std::string& strGBK);
bool Utility_ReplaceString(std::string& str, const std::string& searchString, const std::string& replaceString);

void Utility_split(std::string src, const char* token, VecString& vect);
//只能处理目录:lpPath只能是路径
void Utility_Find(const char *lpPath, VecString &vecFiles, bool bRecus = true, const char *pExt = "*.*");

void Utility_GetStringFromFile(const std::string &name, std::string &value);
unsigned char* Utility_GetDataFromFile(const std::string &name, long &nSize);

void Utility_SaveStringToFile(const std::string& value, const std::string& name);
void Utility_SaveDataToFile(void *data, long nSize, const std::string& name);

std::string GetFileDir(const std::string& pFile);
std::string GetFileName(const std::string& pFile, bool HasExt = true);

int CreatDir(char *pszDir);

template<typename T>
void Utility_ReadValue(T* Value, void *pData, int *pPos)
{
    *Value = *(T*)((char*)pData+*pPos);
    *pPos = *pPos + sizeof(T);
}
void Utility_ReadNameInt(char **pName, void *pData, int *pPos);
void Utility_ReadNameChar(char **pName, void *pData, int *pPos);
void Utility_ReadData(void* data, void *pData, int nSize, int *pPos);

template<typename T>
void Utility_WriteValue(T* Value, FILE *fp)
{
    fwrite(Value, sizeof(T), 1, fp);
}
void Utility_WriteNameInt(char *pName, FILE *fp);

void Utility_CreateProcess(std::string strCommand);

const std::string& Utility_NormaliseDir(std::string& strDir);

#endif
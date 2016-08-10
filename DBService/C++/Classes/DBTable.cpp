#include "DBTable.h"
#include <io.h>
//-----------------------------------------------------
// ¹¹Ôì
//-----------------------------------------------------
DBTable::DBTable(void)
: m_Data(NULL)
, m_nKeyIndex(-1)
{
}

//-----------------------------------------------------
// Îö¹¹
//-----------------------------------------------------
DBTable::~DBTable(void)
{
    if (m_Data)
        free(m_Data);
}

bool DBTable::Load(const char *pcFileName)
{
	m_strFileName = pcFileName;

    FILE *fp = fopen(pcFileName, "rb");
    if (fp == NULL)
        return false;

    long nSize = filelength(fileno(fp));
    char *pData = (char*)malloc(nSize);
    fread(pData, nSize, 1, fp);
    fclose(fp);

    LoadFromMemory(pData, nSize);

    free(pData);

	return true;
}

bool DBTable::LoadFromMemory(const char *pData, int nCount/* = -1*/)
{
    if (nCount == -1)
        nCount = strlen(pData);

    m_Record.clear();
    m_Data = strdup(pData);

    bool bStrStar = false;

    const char *pStr = m_Data;
    VecCol *pCol = new VecCol();
    for (int i = 0; i < nCount; i++)
    {
        char* ch = m_Data + i;
        unsigned char *uch = (unsigned char*)ch;
        if (*uch < 0xC0)
        {
            switch (*ch)
            {
                case '"':
                    if (*pStr == '"' && pStr != ch)
                    {
                        if (*(ch + 1) == '"')
                        {
                            i++;
                            break;
                        }
                        else if (*(ch + 1) == ',')
                        {
                            pStr++;
                            *ch = '\0';
                            i++;
                            pCol->push_back(pStr);
                            pStr = m_Data + i + 1;
                            break;
                        }
                        else if (*(ch + 1) == '\n')
                        {
                            pStr++;
                            *ch = '\0';
                            i++;
                            pCol->push_back(pStr);
                            pStr = m_Data + i + 1;
                            m_Record.push_back(pCol);
                            pCol = new VecCol();
                            break;
                        }
                    }
                    break;
                case ',':
                    if (pStr == ch)
                    {
                        pStr = "";
                        pCol->push_back(pStr);
                    }
                    else if (*pStr != '"')
                    {
                        *ch = '\0';
                        pCol->push_back(pStr);
                        pStr = m_Data + i + 1;
                    }
                    break;
                case '\n':
                    if (*pStr != '"')
                    {
                        *ch = '\0';
                        pCol->push_back(pStr);
                        pStr = m_Data + i + 1;
                        m_Record.push_back(pCol);
                        pCol = new VecCol();
                    }
                    break;
            }
        }
        else if (*uch < 0xE0)
        {
            i++;
        }
        else if (*uch < 0xF0)
        {
            i += 2;
        }
        else if (*uch < 0xF8)
        {
            i += 3;
        }
        else if (*uch < 0xFC)
        {
            i += 4;
        }
        else
        {
            i += 5;
        }
    }
    if (pCol->size() == 0)
        delete pCol;

    if (m_Record.size() < 2)
        return false;

    VecCol &vecHead = *m_Record[0];
    for (size_t i = 0; i < vecHead.size(); i++)
    {
        m_MapKey[vecHead[i]] = i;
    }

    return true;
}

void DBTable::MakeKey(const char *pKey)
{
    MapKey::iterator it = m_MapKey.find(pKey);
    if (it == m_MapKey.end())
    {
        return ;
    }
    MakeKey(it->second);
}

void DBTable::MakeKey(int nColIndex)
{
    if (m_nKeyIndex == nColIndex)
        return;
    m_nKeyIndex = nColIndex;
    for (size_t i = 2; i < m_Record.size(); i++)
    {
        DBRecord *pRecord = new DBRecord(&m_MapKey, m_Record[i]);
        const char *pKey = (*m_Record[i])[nColIndex];
        if (strlen(pKey) != 0)
        {
            m_RecordMap[pKey] = pRecord;
        }
    }
}

DBRecord *DBTable::FindRecord(int nID)
{
    char szID[20];
    sprintf(szID, "%d", nID);
    return FindRecord(szID);
}

DBRecord *DBTable::FindRecord(const char *pKey)
{
    RECORD_STR_MAP::iterator it = m_RecordMap.find(pKey);
    if (it == m_RecordMap.end())
        return NULL;

    return it->second;
}
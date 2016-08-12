#include "DBRecord.h"

//-----------------------------------------------------
// ¹¹Ôì
//-----------------------------------------------------
DBRecord::DBRecord(MapKey *pMapKey, VecCol *pData)
: m_pMapKey(pMapKey)
, m_Data(pData)
{
}

//-----------------------------------------------------
// Îö¹¹
//-----------------------------------------------------
DBRecord::~DBRecord(void)
{
	free(m_pBlock);
}

const char *DBRecord::Attribute(const char *pKey)
{
    MapKey::iterator it = (*m_pMapKey).find(pKey);
    if (it == m_pMapKey->end())
    {
        return "";
    }
    return (*m_Data)[it->second];
}

int DBRecord::AttributeInt(const char *pKey)
{
    const char *pValue = Attribute(pKey);
    int value;
    sscanf_s(pValue, "%d", &value);
    return value;
}

unsigned int DBRecord::AttributeUInt(const char *pKey)
{
    const char *pValue = Attribute(pKey);
    unsigned int value;
    sscanf_s(pValue, "%u", &value);
    return value;
}

float DBRecord::AttributeFloat(const char *pKey)
{
    const char *pValue = Attribute(pKey);
    float value;
    sscanf_s(pValue, "%f", &value);
    return value;
}
double DBRecord::AttributeDouble(const char *pKey)
{
    const char *pValue = Attribute(pKey);
    double value;
    sscanf_s(pValue, "%lf", &value);
    return value;
}

IDBBlock* DBRecord::Block()
{
	return m_pBlock;
}

void DBRecord::BuildBlock(VecCol &vecCol)
{
	int nBlockSize = vecCol.size()*4;
	char *pBlock = (char*)malloc(nBlockSize);
	m_pBlock = (IDBBlock*)pBlock;
	
	int nIndex = 0;
	for(int i = 0; i < vecCol.size(); i++)
	{
		const char *pType = vecCol[i];
		switch(pType[0])
		{
		case 'i':
			{
				sscanf_s((*m_Data)[i], "%d", (int*)(pBlock+nIndex));
				nIndex += sizeof(int);
			}
			break;
		case 's':
			{
				*(const char **)(pBlock+nIndex) = (*m_Data)[i];
				nIndex += sizeof(char*);
			}
			break;
		case 'f':
			{			
				sscanf_s((*m_Data)[i], "%f", (float*)(pBlock+nIndex));
				nIndex += sizeof(float);
			}
			break;
		}
	}
}
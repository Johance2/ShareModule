#ifndef _DBTable_H_
#define _DBTable_H_
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma warning(disable:4996)

#include "../Interface/IDBTable.h"
#include "DBRecord.h"
#include <map>
#include <string>

class DBTable : public IDBTable
{
public:
	DBTable();
	virtual ~DBTable(void);

    virtual bool Load(const char *pcFileName);
    virtual bool LoadFromMemory(const char *pData, int nCount = -1);
    void MakeKey(const char *pKey);
    void MakeKey(int nColIndex);
    virtual DBRecord *FindRecord(int nID);
    virtual DBRecord *FindRecord(const char *pKey);
	virtual IDBRecord *FindRecordByIndex(int nIndex);
    virtual IDBBlock* FindData(int nID);
    virtual IDBBlock* FindData(const char *pKey);
	virtual IDBBlock* FindDataByIndex(int nIndex);
	virtual size_t GetRecountCount();


protected:
    VecRow m_Record;
    MapKey m_MapKey;
    int m_nKeyIndex;
    typedef std::map<std::string, DBRecord*> RECORD_STR_MAP;
    RECORD_STR_MAP m_RecordMap;
    typedef std::vector<DBRecord*> RECORD_VEC;
	RECORD_VEC m_RecordVec;

private:
    std::string m_strFileName;
    char *m_Data;

};

#endif
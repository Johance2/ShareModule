#ifndef _DBRecord_H_
#define _DBRecord_H_

#include "../Interface/IDBRecord.h"
#include <string>
#include <map>
#include <vector>
#include <unordered_map>

typedef std::unordered_map<std::string, int> MapKey;
typedef std::vector<const char*> VecCol;
typedef std::vector<VecCol*> VecRow;

class DBRecord : public IDBRecord
{
public:
    DBRecord(MapKey *pMapKey, VecCol *pData);
	virtual ~DBRecord(void);

    virtual const char *Attribute(const char *pKey);
    virtual int AttributeInt(const char *pKey);
    virtual unsigned int AttributeUInt(const char *pKey);
    virtual float AttributeFloat(const char *pKey);
    virtual double AttributeDouble(const char *pKey);
	virtual IDBBlock* Block();
	void BuildBlock(VecCol &vecCol);

protected:
    MapKey *m_pMapKey;
    VecCol *m_Data;
	IDBBlock *m_pBlock;
};

#endif
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

    const char *Attribute(const char *pKey);
    int AttributeInt(const char *pKey);
    unsigned int AttributeUInt(const char *pKey);
    float AttributeFloat(const char *pKey);
    double AttributeDouble(const char *pKey);

protected:
    MapKey *m_pMapKey;
    VecCol *m_Data;
};

#endif
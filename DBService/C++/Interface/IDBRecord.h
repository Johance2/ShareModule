#ifndef _IDBRecord_H_
#define _IDBRecord_H_

class IDBRecord
{
public:
    virtual const char *Attribute(const char *pKey) = 0;
    virtual int AttributeInt(const char *pKey) = 0;
    virtual unsigned int AttributeUInt(const char *pKey) = 0;
    virtual float AttributeFloat(const char *pKey) = 0;
    virtual double AttributeDouble(const char *pKey) = 0;
};

#endif
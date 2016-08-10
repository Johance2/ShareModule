#ifndef _IDataStream_H_
#define _IDataStream_H_

#include <string>
#include "BaseType.h"

class IDataStream
{
public:
	virtual bool isReadable() const = 0;
	virtual bool isWriteable() const = 0;
	virtual size_t read(void* buf, size_t count) = 0;
	virtual size_t write(const void* buf, size_t count) = 0;
	virtual size_t readLine(char* buf, size_t maxCount, const string& delim = "\n") = 0;
	virtual string getLine( bool trimAfter = true ) = 0;
	virtual string getAsString(void) = 0;
	virtual size_t skipLine(const string& delim = "\n") = 0;
	virtual void skip(long count) = 0;
	virtual void seek( size_t pos ) = 0;
	virtual size_t tell(void) const = 0;
	virtual bool eof(void) const = 0;
	virtual size_t size(void) const = 0;
	virtual void close(void) = 0;
	virtual void Release(void) = 0;
};

#endif
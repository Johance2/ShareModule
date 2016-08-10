#pragma once
#include "../Interface/IDataStream.h"

#define STREAM_TEMP_SIZE 128

class DataStream : public IDataStream
{
public:
	enum AccessMode
	{
		READ = 1, 
		WRITE = 2
	};
protected:
	std::string mName;       
	size_t mSize;
	unsigned short mAccess;
public:
	DataStream(USHORT accessMode = READ) : mSize(0), mAccess(accessMode) {}
	DataStream(const string& name, USHORT accessMode = READ) 
		: mName(name), mSize(0), mAccess(accessMode) {}
	const string& getName(void) { return mName; }
	virtual bool isReadable() const { return (mAccess & READ) != 0; }
	virtual bool isWriteable() const { return (mAccess & WRITE) != 0; }
	virtual ~DataStream() {}
	template<typename T> DataStream& operator>>(T& val);
	virtual size_t read(void* buf, size_t count) = 0;
	virtual size_t write(const void* buf, size_t count)
	{
		(void)buf;
		(void)count;
		// default to not supported
		return 0;
	}
	virtual size_t readLine(char* buf, size_t maxCount, const string& delim = "\n");        
	virtual string getLine( bool trimAfter = true );
	virtual string getAsString(void);
	virtual size_t skipLine(const string& delim = "\n");
	virtual void skip(long count) = 0;
	virtual void seek( size_t pos ) = 0;
	virtual size_t tell(void) const = 0;
	virtual bool eof(void) const = 0;
	size_t size(void) const { return mSize; }
	virtual void close(void) = 0;
	virtual void Release(void) {delete this;};
};

class MemoryDataStream : public DataStream
{
protected:
	UCHAR* mData;
	UCHAR* mPos;
	UCHAR* mEnd;
	bool mFreeOnClose;          
public:
	MemoryDataStream(void* pMem, size_t size, bool freeOnClose = false, bool readOnly = false);
	MemoryDataStream(const string& name, void* pMem, size_t size, 
		bool freeOnClose = false, bool readOnly = false);
	MemoryDataStream(DataStream& sourceStream, 
		bool freeOnClose = true, bool readOnly = false);
	MemoryDataStream(const string& name, DataStream& sourceStream, 
		bool freeOnClose = true, bool readOnly = false);
	MemoryDataStream(size_t size, bool freeOnClose = true, bool readOnly = false);
	MemoryDataStream(const string& name, size_t size, 
		bool freeOnClose = true, bool readOnly = false);

	~MemoryDataStream();
	UCHAR* getPtr(void) { return mData; }
	UCHAR* getCurrentPtr(void) { return mPos; }
	size_t read(void* buf, size_t count);

	size_t write(const void* buf, size_t count);
	size_t readLine(char* buf, size_t maxCount, const string& delim = "\n");
	size_t skipLine(const string& delim = "\n");
	void skip(long count);
	void seek( size_t pos );
	size_t tell(void) const;
	bool eof(void) const;
	void close(void);
	void setFreeOnClose(bool free) { mFreeOnClose = free; }
};

class FileStreamDataStream : public DataStream
{
protected:
	std::istream* mInStream;
	std::ifstream* mFStreamRO;
	std::fstream* mFStream;
	bool mFreeOnClose;  

	void determineAccess();
public:
	FileStreamDataStream(std::ifstream* s, 
		bool freeOnClose = true);
	FileStreamDataStream(std::fstream* s, 
		bool freeOnClose = true);
	FileStreamDataStream(const string& name, 
		std::ifstream* s, 
		bool freeOnClose = true);
	FileStreamDataStream(const string& name, 
		std::fstream* s, 
		bool freeOnClose = true);
	FileStreamDataStream(const string& name, 
		std::ifstream* s, 
		size_t size, 
		bool freeOnClose = true);
	FileStreamDataStream(const string& name, 
		std::fstream* s, 
		size_t size, 
		bool freeOnClose = true);

	~FileStreamDataStream();
	size_t read(void* buf, size_t count);
	size_t write(const void* buf, size_t count);
	size_t readLine(char* buf, size_t maxCount, const string& delim = "\n");        
	void skip(long count);
	void seek( size_t pos );
	size_t tell(void) const;
	bool eof(void) const;
	void close(void);
};

class FileHandleDataStream : public DataStream
{
protected:
	FILE* mFileHandle;
public:
	FileHandleDataStream(FILE* handle, USHORT accessMode = READ);
	FileHandleDataStream(const string& name, FILE* handle, USHORT accessMode = READ);
	~FileHandleDataStream();
	size_t read(void* buf, size_t count);
	size_t write(const void* buf, size_t count);
	void skip(long count);
	void seek( size_t pos );
	size_t tell(void) const;
	bool eof(void) const;
	void close(void);
};
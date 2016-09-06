#ifndef _BASIC_TYPE_H_
#define _BASIC_TYPE_H_

#ifdef _WIN32
	#include <WinSock2.h>
	#include <Windows.h>

	#ifndef LLONG
	#define LLONG	LONGLONG
	#endif

	#ifndef ULLONG
	#define ULLONG	ULONGLONG
	#endif

	#ifndef INT64
	#define INT64	LLONG
	#endif

	#ifndef UINT64
	#define UINT64	ULLONG
	#endif

#else
	typedef char						CHAR;
	typedef unsigned char				BYTE;
	typedef unsigned char				UCHAR;
	typedef short						SHORT;
	typedef unsigned short				USHORT;
	typedef unsigned short				WORD;
	typedef unsigned int				UINT;
	typedef int							INT;
	typedef unsigned int				DWORD;
	typedef long long					LLONG;
	typedef unsigned long long			ULLONG;
	typedef void						VOID;
	typedef long long					INT64;
	typedef unsigned long long	UINT64;
	typedef double						DOUBLE;	
	typedef unsigned long long	ULONGLONG;
	typedef long long	LONGLONG;

	typedef	BYTE						*LPBYTE;
	typedef	WORD						*LPWORD;
	typedef	DWORD						*LPDWORD;
	typedef	VOID						*LPVOID;
	typedef	CHAR						*PCHAR;

    typedef	BYTE BOOLEAN;

	#ifndef TRUE
	#define TRUE	1
	#endif

	#ifndef FALSE
	#define FALSE	0
	#endif

	#ifndef NULL
	#define NULL	((void*)0)
	#endif
    
    typedef	float FLOAT;

#endif

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <assert.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <istream>
#include <iostream>
#include <fstream>

using std::string;
using std::vector;
using std::stringstream;
using std::map;

// 定义一个全局的id类型
typedef ULLONG IDTYPE;

#endif

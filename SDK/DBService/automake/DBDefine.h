#ifndef _DBDefine_Automake_H_
#define _DBDefine_Automake_H_

struct IDBBlock{};
enum TableType
{
	TT_Example,
	TT_Amount
};

struct DBData_Example : IDBBlock
{
	int ID;//编号
	const char* strValue;//字符值
	int IntValue;//整型值
	float FloatValue;//浮点值
};

#endif
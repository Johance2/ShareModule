#ifndef _DBDefine_Automake_H_
#define _DBDefine_Automake_H_

enum TableType
{
	TT_Example,
	TT_Amount
};

struct DBData_Example : IDBRecordData
{
	int ID;//编号
	std::string strValue;//字符值
	int IntValue;//整型值
	float FloatValue;//浮点值
};

#endif
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
	int ID;//���
	const char* strValue;//�ַ�ֵ
	int IntValue;//����ֵ
	float FloatValue;//����ֵ
};

#endif
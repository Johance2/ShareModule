#ifndef _DBDefine_Automake_H_
#define _DBDefine_Automake_H_

enum TableType
{
	TT_Example,
	TT_Amount
};

struct DBData_Example : IDBRecordData
{
	int ID;//���
	std::string strValue;//�ַ�ֵ
	int IntValue;//����ֵ
	float FloatValue;//����ֵ
};

#endif
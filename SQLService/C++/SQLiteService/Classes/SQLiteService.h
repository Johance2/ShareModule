/********************************************************************
	purpose:	���ݿ����ӿ�ʵ����
*********************************************************************/

#ifndef _SQLiteService_H_
#define _SQLiteService_H_
#include "../../Common/DataBase.h"

struct sqlite3;

class SQLiteService :
	public DataBase
{
public:
	SQLiteService(IKernel *pKernel);
	virtual ~SQLiteService(void);

	// --------- ʵ�ֻ���ӿ� ------------
	virtual void Init();
	virtual void Shut();
	virtual void Update(float fTime);

public:
    int ExcuteQuery(IQueryResult **result, const char *cmd);
    int ExcuteQuery(const char *cmd);
    int ExcuteBinary(const char *cmd, SQLParam *param);

protected:
    int OpenImpl(std::map<std::string, std::string> &strmap);

private:
    sqlite3 *_db;
};

#endif

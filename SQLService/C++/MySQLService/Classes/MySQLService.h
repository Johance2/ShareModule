/********************************************************************
	purpose:	���ݿ����ӿ�ʵ����
*********************************************************************/

#ifndef _MySQLService_H_
#define _MySQLService_H_
#include "../../Common/DataBase.h"

struct st_mysql;

class MySQLService :
	public DataBase
{
public:
	MySQLService();
	virtual ~MySQLService(void);

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
    st_mysql *_connection;
};

#endif

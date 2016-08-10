#include "ODBCQueryResult.h"

ODBCQueryResult::ODBCQueryResult(SQLHSTMT res, uint64 row_count, uint32 field_count)
: QueryResult(row_count, field_count)
, m_hstmt(res)
{

}

ODBCQueryResult::~ODBCQueryResult(void)
{
}

bool ODBCQueryResult::Read()
{
	SQLRETURN mResult = SQLFetch(m_hstmt);

	if ((mResult != SQL_SUCCESS) && (mResult != SQL_SUCCESS_WITH_INFO) && (mResult != SQL_NO_DATA)) 
	{
		return false;
	}

	if(mResult == SQL_NO_DATA)
	{
		return false;
	}

	return true;
}

void ODBCQueryResult::SetToBegin()
{
	SQLSetPos(m_hstmt, 0, SQL_POSITION, SQL_LOCK_NO_CHANGE);
}

std::string ODBCQueryResult::field_name(int n)
{
	return "";
    //SQLColAttribute(m_hstmt, n, 
    //return (char*)_bstr_t(m_pRecordset->GetFields()->GetItem(long(n))->GetName());
}

uint32 ODBCQueryResult::length(int n)
{
	return 0;
    //m_pRecordset->GetFields()->GetItem((long)n)->GetActualSize();
}

uint32 ODBCQueryResult::get_data(int n, char *data)
{
    char *pBuf = NULL;

    uint32 l = length(n);

    //pBuf = (char*)GlobalAlloc(GMEM_FIXED, l);

    //_variant_t var = m_pRecordset->GetFields()->GetItem((long)n)->GetValue();

    //SafeArrayAccessData(var.parray,(void **)&pBuf);

    //memcpy(data, pBuf, l);
    //
    //SafeArrayUnaccessData(var.parray);

    return l;
}

int32 ODBCQueryResult::get_int32(int n)
{
	return 0;
    //return m_pRecordset->GetFields()->GetItem((long)n)->GetValue().vt != VT_NULL ? (ulong)m_pRecordset->GetFields()->GetItem((long)n)->GetValue() : 0;
}

int64 ODBCQueryResult::get_int64(int n)
{
	return 0;
    //return m_pRecordset->GetFields()->GetItem((long)n)->GetValue().vt != VT_NULL ? m_pRecordset->GetFields()->GetItem((long)n)->GetValue() : 0;
}

float ODBCQueryResult::get_float(int n)
{
	return 0;
    //return m_pRecordset->GetFields()->GetItem((long)n)->GetValue().vt != VT_NULL ? (float)m_pRecordset->GetFields()->GetItem((long)n)->GetValue() : 0;
}

double ODBCQueryResult::get_double(int n)
{
	return 0;
    //return m_pRecordset->GetFields()->GetItem((long)n)->GetValue().vt != VT_NULL ? (double)m_pRecordset->GetFields()->GetItem((long)n)->GetValue() : 0;
}

std::string ODBCQueryResult::get_string(int n)
{
	return 0;
    //return  m_pRecordset->GetFields()->GetItem(long(n))->GetValue().vt != VT_NULL ? (char*)_bstr_t(m_pRecordset->GetFields()->GetItem(long(n))->GetValue()) : "";
}

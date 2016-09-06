#ifndef __PushBox_H__
#define __PushBox_H__

#include "../Interface/IPushBox.h"
#include <windows.h>

#define MAX_LOADSTRING 100

class PushBox : public IPushBox
{
public:
    PushBox();
    ~PushBox();
	
	virtual void Init(ModuleAttribute *pAttribute);
	virtual void Shut();
	virtual void Update(float fTime);
	
public:
	void ResigterClass();
	BOOL InitInstance();

private:
	HINSTANCE m_hInstance;
	TCHAR m_szWindowClass[MAX_LOADSTRING];// 主窗口类名
	TCHAR m_szTitle[MAX_LOADSTRING];					// 标题栏文本
	HACCEL m_hAccelTable;
};

#define g_pPushBox ((PushBox*)PushBoxEnv::GetPushBoxModule(eGMIT_PushBox))

#endif // __PushBox_H__
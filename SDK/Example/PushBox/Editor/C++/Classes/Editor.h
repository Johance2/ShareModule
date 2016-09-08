#ifndef __Editor_H__
#define __Editor_H__

#include "../Interface/IEditor.h"
#include <windows.h>

#define MAX_LOADSTRING 100
#define MAP_X (100)
#define MAP_Y (0)

class Editor : public IEditor
{
public:
    Editor();
    ~Editor();
	
	virtual void Init(ModuleAttribute *pAttribute);
	virtual void Shut();
	virtual void Update(float fTime);
	
public:
	void ResigterClass();
	BOOL InitInstance();

private:
	HINSTANCE m_hInstance;
	TCHAR m_szWindowClass[MAX_LOADSTRING];// ����������
	TCHAR m_szTitle[MAX_LOADSTRING];					// �������ı�
	HACCEL m_hAccelTable;
	int m_nWidth;
	int m_nHeight;
};

#define g_pEditor ((Editor*)EditorEnv::GetEditorModule(eGMIT_Editor))

#endif // __Editor_H__
#include "Editor.h"
#include <assert.h>
#include "ILog.h"
#include "resource.h"
#include "..\..\..\Game\C++\Interface\IGame.h"

HINSTANCE g_hInstance;
HWND g_hWnd;
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

IGame *g_pGame = NULL;
int g_Type = 1;

Editor::Editor()
{
}

Editor::~Editor()
{
}

void Editor::Init(ModuleAttribute *pAttribute)
{		
	m_nWidth = atoi((*pAttribute)["Width"].c_str());
	m_nHeight = atoi((*pAttribute)["Height"].c_str());
	g_pGame = (IGame*)g_pKernel->FindModule(GameModuleID);
	g_hInstance = m_hInstance = g_pKernel->FindModuleHandle(EditorModuleID);
	// 初始化全局字符串
	LoadString(g_hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
	LoadString(g_hInstance, IDC_EDITOR, m_szWindowClass, MAX_LOADSTRING);

	ResigterClass();
	if(!InitInstance())
	{
		LOGFMTE("InitInstance Failuer!");
		return ;
	}
	m_hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_EDITOR));
}

void Editor::Shut()
{
}

void Editor::Update(float fTime)
{
	MSG msg;
    if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) 
    { 
        if (msg.message == WM_QUIT) 
		{
			g_pKernel->Quit();
			return ;
		}
        TranslateMessage (&msg) ; 
        DispatchMessage (&msg) ; 
    } 
    else 
    { 
    // 完成某些工作的其他行程式 
    } 
}

void Editor::ResigterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon			= LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_EDITOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_EDITOR);
	wcex.lpszClassName	= m_szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);
}

BOOL Editor::InitInstance()
{
   g_hWnd = CreateWindow(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, m_nWidth, m_nHeight, NULL, NULL, m_hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }
   
   g_pGame->SetHWND(g_hWnd);
   g_pGame->Load(1);
   ShowWindow(g_hWnd, SW_SHOW);
   UpdateWindow(g_hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(g_hWnd);
			break;
		case IDM_SAVE:
			g_pGame->Save("editor.map");
			break;
		case IDM_LOAD:
			g_pGame->Load("editor.map");
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		{
		}
		break;
	case WM_MOUSEMOVE:
		{
			if(!(wParam & (MK_LBUTTON|MK_RBUTTON)))
			{
				break;
			}
		}
	case WM_LBUTTONDOWN:
		{
			int nX = LOWORD(lParam);
			int nY = HIWORD(lParam);

			if(nX < 50 && nX > 0 && nY < 250 && nY > 0)
			{
				int nIndex = nY / 50;

				if(nIndex != 5)
				{
					g_Type = nIndex;
				}
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else if(nX > MAP_X && nX < (MAP_X+MAP_WIDTH) && nY > MAP_Y && nY < (MAP_Y+MAP_HEIGHT))
			{
				int nCol = (nX - MAP_X) / TILE_WIDTH;
				int nRow = (nY - MAP_Y) / TILE_HEIGHT;

				int nType = g_Type;
				if(wParam & MK_RBUTTON)
				{
					nType = 0;
				}

				g_pGame->Change(nCol, nRow, nType);
			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for(int i = 0; i < 5; i++)
		{			
			g_pGame->DrawItem(0, TILE_HEIGHT*i, i);
		}
		
		g_pGame->DrawItem(0, TILE_HEIGHT*6, g_Type);

		g_pGame->Draw(MAP_X, MAP_Y);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

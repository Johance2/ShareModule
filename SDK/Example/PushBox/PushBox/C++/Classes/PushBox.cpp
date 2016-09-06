#include "PushBox.h"
#include <assert.h>
#include "ILog.h"
#include "resource.h"
#include "..\..\..\Game\C++\Interface\IGame.h"

HINSTANCE g_hInstance;
HWND g_hWnd;
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

IGame *g_pGame = NULL;

PushBox::PushBox()
{
}

PushBox::~PushBox()
{
}

void PushBox::Init(ModuleAttribute *pAttribute)
{	
	g_pGame = (IGame*)g_pKernel->FindModule(GameModuleID);
	g_hInstance = m_hInstance = g_pKernel->FindModuleHandle(PushBoxModuleID);
	// 初始化全局字符串
	LoadString(g_hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
	LoadString(g_hInstance, IDC_PUSHBOX, m_szWindowClass, MAX_LOADSTRING);


	ResigterClass();
	if(!InitInstance())
	{
		LOGFMTE("InitInstance Failuer!");
		return ;
	}
	m_hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_PUSHBOX));
}

void PushBox::Shut()
{
}

void PushBox::Update(float fTime)
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

void PushBox::ResigterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon			= LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_PUSHBOX));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PUSHBOX);
	wcex.lpszClassName	= m_szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);
}

BOOL PushBox::InitInstance()
{
   g_hWnd = CreateWindow(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, m_hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }
   
   g_pGame->SetHWND(g_hWnd);

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
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		{
			g_pGame->Load(1);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		g_pGame->Draw(0, 0);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_UP:
				{
					g_pGame->TurnUp();
				}
				break;
			case VK_DOWN:
				{
					g_pGame->TurnDown();
				}
				break;
			case VK_LEFT:
				{
					g_pGame->TurnLeft();
				}
				break;
			case VK_RIGHT:
				{
					g_pGame->TurnRight();
				}
				break;
			}
		}
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

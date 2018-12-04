// MIUI-Theme-Download-Link-Generator.cpp : 定义应用程序的入口点。
//

#include "header.h"
#include "MIUI-Theme-Download-Link-Generator.h"
#include "generate.h"							//解析直链

#pragma comment(lib, "WINMM.LIB")//播放声音

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HWND BUTTON_OPENSTORE;
HWND BUTTON_GENERATE;
HWND BUTTON_COPY;
HWND BUTTON_DOWNLOAD;
HWND BUTTON_ABOUTME;

static HWND EDIT_STOREUTL;
static HWND EDIT_EDIT_DOWNLINK;




// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MIUITHEMEDOWNLOADLINKGENERATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MIUITHEMEDOWNLOADLINKGENERATOR));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MIUITHEMEDOWNLOADLINKGENERATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MIUITHEMEDOWNLOADLINKGENERATOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, L"MIUI主题下载直链", WS_SYSMENU | WS_MINIMIZEBOX,
      320, 180, 574, 158, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HFONT hFont ;
	static HFONT hFontButton;
	LPWSTR urlTheme;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case ID_BUTTON_ABOUTME:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case ID_BUTTON_OPENSTORE:
				ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("http://zhuti.xiaomi.com/"), NULL, SW_SHOW);
				break;
			case ID_BUTTON_GENERATE:
				urlTheme = new wchar_t[1000];
				GetWindowText(EDIT_STOREUTL, urlTheme, 1000);
				if (Generate(urlTheme))
				{
					PlaySound((LPCTSTR)IDR_WAVE_SUCCESS, hInst, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
				}
				else
				{
					PlaySound((LPCTSTR)IDR_WAVE_FAILED, hInst, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
				}
				SetWindowText(EDIT_EDIT_DOWNLINK, urlTheme);
				delete []urlTheme;
				break;
			case ID_BUTTON_COPY:
				urlTheme = new wchar_t[1000];
				GetWindowText(EDIT_STOREUTL, urlTheme, 1000);
				if (Generate(urlTheme) && CopyToClipboard(hWnd, urlTheme, 998))
				{
					PlaySound((LPCTSTR)IDR_WAVE_SUCCESS, hInst, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
				}
				else
				{
					PlaySound((LPCTSTR)IDR_WAVE_FAILED, hInst, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
				}
				SetWindowText(EDIT_EDIT_DOWNLINK, urlTheme);
				delete[]urlTheme;
				break;
			case ID_BUTTON_DOWNLOAD:
				urlTheme = new wchar_t[1000];
				GetWindowText(EDIT_STOREUTL, urlTheme, 1000);
				if (Generate(urlTheme))
				{
					SetWindowText(EDIT_EDIT_DOWNLINK, urlTheme);
					ShellExecute(NULL, _T("open"), _T("explorer.exe"), urlTheme, NULL, SW_SHOW);
				}
				else
				{
					SetWindowText(EDIT_EDIT_DOWNLINK, urlTheme);
					PlaySound((LPCTSTR)IDR_WAVE_FAILED, hInst, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
				}
				delete[]urlTheme;
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			//MoveToEx(hdc, 0, 0, NULL);
			//LineTo(hdc, 575, 0);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CREATE:
		{
			hFont = CreateFont(-20/*高*/, -10/*宽*/, 0, 0, 0 /*700表示粗体*/,FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET,OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,FF_DONTCARE, TEXT("微软雅黑"));

			EDIT_STOREUTL		= CreateWindow(L"EDIT", L"粘贴主题链接", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL , 7,  8, 542, 28, hWnd, (HMENU)ID_EDIT_STOREUTL, NULL, NULL);
			SendMessage(EDIT_STOREUTL, WM_SETFONT, (WPARAM)hFont, 1);

			EDIT_EDIT_DOWNLINK	= CreateWindow(L"EDIT", L"生成的下载链接", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL , 7, 43, 542, 28, hWnd, NULL, NULL, NULL);
			SendMessage(EDIT_EDIT_DOWNLINK, WM_SETFONT, (WPARAM)hFont, 1);

			hFontButton = CreateFont(-16/*高*/, -7/*宽*/, 0, 0, 0 /*700表示粗体*/, FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, TEXT("微软雅黑"));

			BUTTON_OPENSTORE	= CreateWindow(L"BUTTON", L"打开主题商店", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_CENTER, 6, 78, 130, 35, hWnd, (HMENU)ID_BUTTON_OPENSTORE, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			SendMessage(BUTTON_OPENSTORE, WM_SETFONT, (WPARAM)hFontButton, 1);

			BUTTON_GENERATE		= CreateWindow(L"BUTTON", L"生成链接", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_CENTER , 140, 78, 110, 35, hWnd, (HMENU)ID_BUTTON_GENERATE, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			SendMessage(BUTTON_GENERATE, WM_SETFONT, (WPARAM)hFontButton, 1);

			BUTTON_COPY = CreateWindow(L"BUTTON", L"复制", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_CENTER, 254, 78, 96, 35, hWnd, (HMENU)ID_BUTTON_COPY, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			SendMessage(BUTTON_COPY, WM_SETFONT, (WPARAM)hFontButton, 1);

			BUTTON_DOWNLOAD		= CreateWindow(L"BUTTON", L"下载", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_CENTER, 354, 78, 96, 35, hWnd, (HMENU)ID_BUTTON_DOWNLOAD, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			SendMessage(BUTTON_DOWNLOAD, WM_SETFONT, (WPARAM)hFontButton, 1);

			BUTTON_ABOUTME		= CreateWindow(L"BUTTON", L"关于", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_CENTER, 454, 78, 97, 35, hWnd, (HMENU)ID_BUTTON_ABOUTME, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			SendMessage(BUTTON_ABOUTME, WM_SETFONT, (WPARAM)hFontButton, 1);

		}
		break;
    case WM_DESTROY:
		DeleteObject(hFont);
		DeleteObject(hFontButton);
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
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL )
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
		else
		if (LOWORD(wParam) == IDC_BUTTON_INDEX)
		{
			ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("https://github.com/BDZNH/MIUI-Theme-Download-Link-Generator/"), NULL, SW_SHOW);
		}
        break;
    }
    return (INT_PTR)FALSE;
}


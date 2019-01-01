// ProcessBar.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ProcessBar.h"
BOOL state;
//#define MAX_LOADSTRING 100
//
//// 全局变量:
//HINSTANCE hInst;                                // 当前实例
//WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
//WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
//
//// 此代码模块中包含的函数的前向声明:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPWSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    // TODO: 在此处放置代码。
//
//    // 初始化全局字符串
//    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//    LoadStringW(hInstance, IDC_PROCESSBAR, szWindowClass, MAX_LOADSTRING);
//    MyRegisterClass(hInstance);
//
//    // 执行应用程序初始化:
//    if (!InitInstance (hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROCESSBAR));
//
//    MSG msg;
//
//    // 主消息循环:
//    while (GetMessage(&msg, nullptr, 0, 0))
//    {
//        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//    }
//
//    return (int) msg.wParam;
//}
//
//
//
////
////  函数: MyRegisterClass()
////
////  目标: 注册窗口类。
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//    WNDCLASSEXW wcex;
//
//    wcex.cbSize = sizeof(WNDCLASSEX);
//
//    wcex.style          = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc    = WndProc;
//    wcex.cbClsExtra     = 0;
//    wcex.cbWndExtra     = 0;
//    wcex.hInstance      = hInstance;
//    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROCESSBAR));
//    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
//    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
//    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROCESSBAR);
//    wcex.lpszClassName  = szWindowClass;
//    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//    return RegisterClassExW(&wcex);
//}
//
////
////   函数: InitInstance(HINSTANCE, int)
////
////   目标: 保存实例句柄并创建主窗口
////
////   注释:
////
////        在此函数中，我们在全局变量中保存实例句柄并
////        创建和显示主程序窗口。
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   hInst = hInstance; // 将实例句柄存储在全局变量中
//
//   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}
//
////
////  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  目标: 处理主窗口的消息。
////
////  WM_COMMAND  - 处理应用程序菜单
////  WM_PAINT    - 绘制主窗口
////  WM_DESTROY  - 发送退出消息并返回
////
////
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_COMMAND:
//        {
//            int wmId = LOWORD(wParam);
//            // 分析菜单选择:
//            switch (wmId)
//            {
//            case IDM_ABOUT:
//                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//                break;
//            case IDM_EXIT:
//                DestroyWindow(hWnd);
//                break;
//            default:
//                return DefWindowProc(hWnd, message, wParam, lParam);
//            }
//        }
//        break;
//    case WM_PAINT:
//        {
//            PAINTSTRUCT ps;
//            HDC hdc = BeginPaint(hWnd, &ps);
//            // TODO: 在此处添加使用 hdc 的任何绘图代码...
//            EndPaint(hWnd, &ps);
//        }
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
//
//// “关于”框的消息处理程序。
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}

      //InitCommonControls

#include "download.h"
#pragma comment(lib, "comctl32") //InitCommonControls

extern long ddlnow;

//extern double ddlnow;


//过程函数的返回值都是LRESULT,类型都是CALLBACK
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	HWND hwnd;        //主窗口句柄,在CreateWindow中赋值
	MSG msg;        //消息变量,在GetMessage中使用
	WNDCLASS wndclass;        //窗口类
	const TCHAR* szAppName = L"ProgressBar";    //类名和窗口名
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);    //窗口背景
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);        //鼠标
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);    //图标
	wndclass.lpszClassName = szAppName;        //类名
	wndclass.cbClsExtra = 0;        //类的额外参数
	wndclass.cbWndExtra = 0;        //窗口的额外参数.用于基于同一窗口类的窗口各自区分.
									//在自定义对话框类时必须指定为DLGWINDOWEXTRA的大小
	wndclass.lpszMenuName = NULL;    //菜单名.可以用作子窗口的id
	wndclass.style = CS_HREDRAW | CS_VREDRAW;    //窗口风格
	wndclass.lpfnWndProc = WndProc;        //窗口过程
	wndclass.hInstance = hInstance;        //包含窗口过程的实例句柄

	if (!RegisterClass(&wndclass))        //注册窗口类
		return 0;
	hwnd = CreateWindow(                //创建窗口
		szAppName,                //窗口类名
		szAppName,                //窗口标题
		WS_OVERLAPPEDWINDOW,    //窗口风格
		CW_USEDEFAULT,            //初始的x坐标
		CW_USEDEFAULT,            //初始的y坐标
		CW_USEDEFAULT,            //初始的宽度
		CW_USEDEFAULT,            //初始的高度
		NULL,                    //父窗口
		NULL,                    //菜单
		hInstance,                //和窗口相关的实例句柄
		NULL                    //额外参数
	);
	ShowWindow(hwnd, nCmdShow);        //显示窗口
	UpdateWindow(hwnd);                //更新窗口
	while (GetMessage(&msg, NULL, 0, 0))    //消息循环
	{
		TranslateMessage(&msg);            //将WM_XXXKEYXXX消息翻译为WM_CHAR消息
		DispatchMessage(&msg);            //传递消息到窗口过程
	}
	return msg.wParam;                    //返回
}

DWORD WINAPI PBThreadProc(LPVOID lpParameter)
{
	HWND hwndPB = (HWND)lpParameter;    //进度条的窗口句柄
	PBRANGE range;                        //进度条的范围

	SendMessage(hwndPB, PBM_SETRANGE,    //设置进度条的范围
		(WPARAM)0, (LPARAM)(MAKELPARAM(0, 100)));

	SendMessage(hwndPB, PBM_GETRANGE,    //获取进度条的范围
		(WPARAM)TRUE,                    //TRUE 表示返回值为范围的最小值,FALSE表示返回最大值
		(LPARAM)&range);
	static int i = 0;
	while (state)
	{
		SendMessage(hwndPB, PBM_SETPOS,(WPARAM)(ddlnow), (LPARAM)0);
		if (SendMessage(hwndPB, PBM_GETPOS, (WPARAM)0, (LPARAM)0) //取得进度条当前位置
			== range.iHigh)
		{
			SendMessage(hwndPB, PBM_SETPOS, (WPARAM)range.iLow, (LPARAM)0); //将进度条复位
			i = 0;
		}
		//UpdateWindow(hwndPB);
		Sleep(20);
		i = i + 1;
	}

	SendMessage(hwndPB, PBM_SETPOS, //设置进度条的新位置为当前位置加上范围的1/20
		(WPARAM)(0), (LPARAM)0);
	i = 0;
	return 0;
}




LRESULT WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndPB;
	static HWND hwndButtonStart;
	static HWND hwndButtonStop;
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDC_BUTTONStart:
			if (state)
			{
				state = false;
				Sleep(50);
			}
			state = true;
			CreateThread(   /*操作进度条的线程*/NULL, 0, (LPTHREAD_START_ROUTINE)PBThreadProc, hwndPB, 0, 0);
			CreateThread(   /*操作进度条的线程*/NULL, 0, (LPTHREAD_START_ROUTINE)download, hwndPB, 0, 0);
			//download();
			break;
		case IDC_BUTTONStop:
			state = false;
			break;
		case IDM_ABOUT:
		    //DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		    break;
		case IDM_EXIT:
		    DestroyWindow(hwnd);
		    break;
		default:
		    return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
	{
		InitCommonControls(); //确保已注册了进度条类PROGRESS_CLASS
		hwndPB = CreateWindowEx(0,PROGRESS_CLASS,NULL,WS_CHILD | WS_VISIBLE,0, 0, 0, 0,/*位置和大小在WM_SIZE中设置*/hwnd,(HMENU)0,((LPCREATESTRUCT)lParam)->hInstance,NULL);
		hwndButtonStart = CreateWindow(L"BUTTON", L"下载",  WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,/*位置和大小在WM_SIZE中设置*/hwnd, (HMENU)IDC_BUTTONStart, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hwndButtonStop = CreateWindow(L"BUTTON", L"停止", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,/*位置和大小在WM_SIZE中设置*/hwnd, (HMENU)IDC_BUTTONStop, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hwndButtonStop, WM_SETFONT, (WPARAM)hFont, 1);
		SendMessage(hwndButtonStart, WM_SETFONT, (WPARAM)hFont, 1);
		//CreateThread(   /*操作进度条的线程*/NULL,0,(LPTHREAD_START_ROUTINE)PBThreadProc,hwndPB,0,0);
		return 0;
	}
	case WM_SIZE:
	{
		RECT rc;
		GetClientRect(hwnd, &rc);
		MoveWindow(
			hwndPB,
			rc.left,
			(rc.bottom - rc.top) / 4,
			rc.right - rc.left,
			(rc.bottom - rc.top) / 10,
			TRUE
		);

		MoveWindow(
			hwndButtonStart,
			rc.right / 4,
			(rc.bottom - rc.top) / 8,
			(rc.right - rc.left)/2,
			(rc.bottom - rc.top) / 10,
			TRUE
		);

		MoveWindow(
			hwndButtonStop,
			rc.right / 4,
			(rc.bottom - rc.top) / 2,
			rc.right / 2,
			(rc.bottom - rc.top) / 10,
			TRUE
		);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


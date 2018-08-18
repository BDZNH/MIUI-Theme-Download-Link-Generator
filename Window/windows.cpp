//#define _CRT_SECURE_NO_WARNINGS
//#include <graphics.h>
//#include <conio.h>
//#include <stdio.h>
//
//int main()
//{
//	// 初始化图形窗口
//	initgraph(640, 480);
//
//	// 定义字符串缓冲区，并接收用户输入
//	char s[10];
//	InputBox(s, 10, "请输入半径");
//
//	// 将用户输入转换为数字
//	int r;
//	sscanf(s, "%d", &r);
//
//	// 画圆
//	circle(320, 240, r);
//
//	// 按任意键退出
//	_getch();
//	closegraph();
//}



//#include <nana/gui.hpp>
//int main()
//{
//	using namespace nana;
//	form fm;
//
//	/*drawing{ fm }.draw([](paint::graphics& graph) {
//		graph.string({ 20, 20 }, "Click on the form to show inputbox");
//	});*/
//	inputbox::text name("<bold blue>Name</>", "Nana C++ Library");	//The format text is also available, the second parameter can be given for default value.
//	inputbox::text gender("Gender", std::vector<std::string>{ "Male", "Female" });
//	inputbox::date birth("Date of birth");
//	inputbox::real height("Height(cm)", 100, 1, 300, 1);
//	inputbox::integer kids("Kids", 0, 0, 100, 1);
//	fm.show();
//
//	internationalization i18n;
//	//Translate these 2 words into Chinese.
//	i18n.set("NANA_BUTTON_OK", u8"确定");
//	i18n.set("NANA_BUTTON_CANCEL", u8"取消");
//
//	//Show an inputbox when the form is clicked.
//	fm.events().click([&fm]
//	{
//		inputbox::text name("<bold blue>Name</>", "Nana C++ Library");	//The format text is also available, the second parameter can be given for default value.
//		inputbox::text gender("Gender", std::vector<std::string>{ "Male", "Female" });
//		inputbox::date birth("Date of birth");
//		inputbox::real height("Height(cm)", 100, 1, 300, 1);
//		inputbox::integer kids("Kids", 0, 0, 100, 1);
//
//		inputbox inbox(fm, "Please input <bold>your personal information</>.", "Personal information");
//
//		//Open the image file
//		paint::image img("inputbox_images.bmp");
//
//		//Use 'copy' to assign the image, these image objects refer to the same
//		//image resource.
//		inbox.image(img, true, { 380, 0, 40, 100 });
//		inbox.image(img, false, { 420, 0, 40, 100 });
//		inbox.image_v(img, true, { 0, 0, 380, 50 });
//		inbox.image_v(img, false, { 0, 50, 380, 50 });
//
//		//Sets a verifier
//		inbox.verify([&name](window handle)
//		{
//			if (name.value().empty())
//			{
//				msgbox mb(handle, "Invalid input");
//				mb << L"Name should not be empty, Please input your name.";
//				mb.show();
//				return false; //verification failed
//			}
//			return true; //verified successfully
//		});
//
//		//Call the show() method to show the inputbox.
//		//This method blocks the execution until the inputbox gets closed.
//		//It returns true if the user clicks OK button, or false otherwise.
//		if (inbox.show(name, gender, birth, height, kids))
//		{
//			auto n = name.value();		//std::string in UTF-8
//			auto g = gender.value();	//std::string in UTF-8
//			auto b = birth.value();		//std::string in UTF-8
//			auto year = birth.year();	//int
//			auto month = birth.month(); //int
//			auto day = birth.day();		//int
//			auto h = height.value();	//double
//			auto k = kids.value();		//int
//		}
//	});
//
//	exec();
//}


//-CreateWditBox - Demo.c
/*C语言Windows程序设计 -> 创建文本输入框 -> 演示*/

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("demo");
	HWND        hwnd;
	MSG            msg;
	WNDCLASS    wndclass;

	wndclass.lpfnWndProc = WndProc;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hInstance = hInstance;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("无法注册窗口类!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("C语言Windows程序设计 -> 创建文本输入框 -> 演示"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndInput;

	switch (message)
	{
	case WM_CREATE:
		hwndInput = CreateWindow(TEXT("edit"), NULL,
			WS_CHILD | WS_VISIBLE  | WS_HSCROLL | WS_BORDER |
			ES_LEFT  | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
			20, 20, 1000, 100,
			hwnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
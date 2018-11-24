// ImGui - standalone example application for DirectX 11
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include "../include/imgui/imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include <Windows.h>
#include "resource.h"

#include "generate.h"

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

HRESULT CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return E_FAIL;

	CreateRenderTarget();

	return S_OK;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX11_InvalidateDeviceObjects();
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
			ImGui_ImplDX11_CreateDeviceObjects();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	// Create application window
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
	wc.hIcon= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassEx(&wc);
	HWND hwnd = CreateWindow(_T("ImGui Example"), _T("MIUI 主题下载链接生成"),
		WS_SYSMENU | \
		WS_MINIMIZEBOX, 100, 100, 1017, 158, NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (CreateDeviceD3D(hwnd) < 0)
	{
		CleanupDeviceD3D();
		UnregisterClass(_T("ImGui Example"), wc.hInstance);
		return 1;
	}

	// Show the window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	// Setup style
	//ImGui::StyleColorsLight();
	ImGui::StyleColorsClassic();

	
	// Load Fonts
	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
	//IM_ASSERT(font != NULL);


	bool show_main_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		// Poll and handle messages (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		// Start the ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(1000, 125), ImGuiCond_FirstUseEver);
		ImGui::Begin("Main Window", &show_main_window, ImGuiWindowFlags_NoTitleBar \
			| ImGuiWindowFlags_NoResize \
			| ImGuiWindowFlags_NoMove);

		// 1. Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		{

			ImGui::Text(u8"Usage：粘贴小米主题的链接到第一个框，点击生成下载链接或者按下Enter，然后点击复制或者下载。by BDZNH");
			static char link[128] = u8"粘贴小米主题的链接";
			static char direct_link[1000] = u8"I'm here";
			ImGui::PushItemWidth(ImGui::GetWindowWidth() - 120);
			ImGui::InputText(u8"小米主题的链接", link, IM_ARRAYSIZE(link));
			ImGui::InputText(u8"生成的主题链接", direct_link, IM_ARRAYSIZE(direct_link));
			ImGui::PopItemWidth();
			ImGui::PushItemWidth(120);

			if (ImGui::Button(u8"打开主题商店"))
			{
				ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("http://zhuti.xiaomi.com/"), NULL, SW_SHOW);
			}
			ImGui::SameLine();
			if (ImGui::Button(u8"生成下载链接"))
			{
				Genarate(link, direct_link);
			}
			ImGui::SameLine();


			//if the Enter key is pressed, generate downloadlink
			for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
			{
				if (io.KeysDownDuration[i] >= 0.0f && 13 == i ) 
				{
					Genarate(link, direct_link);//the main function.
				}
			}
				
			//copy the direct download link to clipboard.
			if (ImGui::Button(u8"复制"))
			{
				Genarate(link, direct_link);
				ImGui::LogToClipboard();
				ImGui::LogText(direct_link);
				ImGui::LogFinish();
			}
			ImGui::SameLine();

			//open the default browser and download the theme.
			if (ImGui::Button(u8"下载"))
			{
				Genarate(link, direct_link);
				ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T(direct_link), NULL, SW_SHOW);
			}
			ImGui::SameLine();

			//access me.
			if (ImGui::Button(u8"联系作者"))
			{
				ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("https://github.com/BDZNH/MIUI-Theme-Download-Link-Generator"), NULL, SW_SHOW);
			}
			ImGui::SameLine();

			ImGui::Text("https://github.com/BDZNH/MIUI-Theme-Download-Link-Generator");
			ImGui::SameLine();

		}

		ImGui::End();

		// Rendering
		ImGui::Render();
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(1, 0); // Present with vsync
		//g_pSwapChain->Present(0, 0); // Present without vsync
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	DestroyWindow(hwnd);
	UnregisterClass(_T("ImGui Example"), wc.hInstance);

	return 0;
}

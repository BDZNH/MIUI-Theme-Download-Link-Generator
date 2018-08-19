#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#define _CRT_SECURE_NO_WARNINGS


//#include "toclipboard.h"
#include "generate.h"
#include <imgui/imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <cstring>
#include <Windows.h>

int tocliboard();

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main()
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(1000, 140, u8"小米主题下载链接生成", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsDark();

	// Load Fonts
	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

	
	bool show_main_window = true;
	bool output_Text_box = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the ImGui frame

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Set the property of the window.
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(1000, 100), ImGuiCond_FirstUseEver);
		ImGui::Begin("Main Window", &show_main_window, ImGuiWindowFlags_NoTitleBar \
			| ImGuiWindowFlags_NoMove \
			| ImGuiWindowFlags_NoResize \
			| ImGuiWindowFlags_ShowBorders \
			| ImGuiWindowFlags_NoMove);
		
		
		static char elevenBytes[1000] = u8"小米主题的链接";
		static char textShowed[1000] = u8"复制下载链接";
		static char author[100] = u8"https://github.com/BDZNH/MIUI-Theme-Download-Link-Generator";
		static float f = 0.0f;

		//ImGui::PushItemWidth(100);
		ImGui::Text(u8"粘贴小米主题的链接"); 
		//ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::InputText("--", elevenBytes, IM_ARRAYSIZE(elevenBytes));
		ImGui::SameLine();
		
		if (ImGui::Button(u8"生成"))
		{
			output_Text_box = true;
		}
		/*ImGui::Text("hahaha");*/
		if (output_Text_box)
		{
			//strcpy(textShowed, elevenBytes);
			Genarate(elevenBytes, textShowed);
			output_Text_box = false;
		}
		ImGui::Text(u8"生成的小米主题链接"); ImGui::SameLine();
		ImGui::InputText("~", textShowed, IM_ARRAYSIZE(textShowed));

		//ImGui::SameLine();

		//if (ImGui::Button(u8"复制"))
		//{
		//	//tocliboard(textShowed);
		//	tocliboard();
		//}

		ImGui::Text(u8"使用说明：把小米主题的链接粘贴到第一个框内，然后点击\"生成\"，主题的下载链接会生成到第二个框内，然后点第一下第二个框并按\"ctrl A\"，");
		ImGui::Text(u8"然后按\"ctrl c\"复制下载链接，到Chrome或者IE的地址栏粘贴，回车即可下载。");
		ImGui::Text(u8"联系作者:  "); ImGui::SameLine();
		ImGui::InputText(" ", author, IM_ARRAYSIZE(author));

		ImGui::End();
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

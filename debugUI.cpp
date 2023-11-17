#include "debugUI.h"

#include "graphical.h"

void DebugUI::Initialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	WIN::Window window = Graphical::GetHwnd();
	const HWND hWnd = window.GetHwnd();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(Graphical::GetDevice().Get(), Graphical::GetDevice().GetContext());

	ImGui::StyleColorsDark();
	//ついでに日本語フォントも追加
	io.Fonts->AddFontFromFileTTF("data/TEXTURE/NotoSansJP-Bold.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	io.MouseDrawCursor = true;
}

void DebugUI::Release()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugUI::BeginDraw()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();
}

void DebugUI::EndDraw()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
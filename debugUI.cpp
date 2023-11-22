/**
 * @file debugUI.cpp
 * @brief DebugUIクラスを実装します。
 * @author Cheung To Hung
 * @date 2023/11/22
 */

#include "debugUI.h"
#include "graphical.h"

/**
 * @brief DebugUIクラスのメソッド
 *
 * このメソッドはDebugUIを初期化します。
 * ImGuiのコンテキストを設定し、Win32とDirectX 11用のImGuiを初期化します。
 * また、スタイルをダークに設定し、ファイルから日本語のフォントを追加します。
 */
void DebugUI::Initialize()
{
	// ImGuiのバージョンをチェック
	IMGUI_CHECKVERSION();
	// ImGuiのコンテキストを作成
	ImGui::CreateContext();
	// ImGuiのIOを取得
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	// ウィンドウハンドルを取得
	WIN::Window window = Graphical::GetHwnd();
	const HWND hWnd = window.GetHwnd();
	// Win32用のImGuiを初期化
	ImGui_ImplWin32_Init(hWnd);
	// DirectX 11用のImGuiを初期化
	ImGui_ImplDX11_Init(Graphical::GetDevice().Get(), Graphical::GetDevice().GetContext());

	// ImGuiのスタイルをダークに設定
	ImGui::StyleColorsDark();
	// ファイルから日本語のフォントを追加
	io.Fonts->AddFontFromFileTTF("data/TEXTURE/NotoSansJP-Bold.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	// マウスカーソルの描画を有効にする
	io.MouseDrawCursor = true;
}

/**
 * @brief このメソッドはDebugUIが使用していたリソースを解放します。
 *
 * DirectX 11とWin32用のImGuiをシャットダウンし、ImGuiのコンテキストを破棄します。
 */
void DebugUI::Release()
{
	// DirectX 11用のImGuiをシャットダウン
	ImGui_ImplDX11_Shutdown();
	// Win32用のImGuiをシャットダウン
	ImGui_ImplWin32_Shutdown();
	// ImGuiのコンテキストを破棄
	ImGui::DestroyContext();
}


/**
 * @brief このメソッドはDebugUIの描画プロセスを開始します。
 *
 * 新しいフレームのためのImGuiを準備します。
 */
void DebugUI::BeginDraw()
{
	// 新しいDirectX 11フレームのためのImGuiを準備
	ImGui_ImplDX11_NewFrame();
	// 新しいWin32フレームのためのImGuiを準備
	ImGui_ImplWin32_NewFrame();
	// 新しいImGuiフレームを開始
	ImGui::NewFrame();
}

/**
 * @brief このメソッドはDebugUIの描画プロセスを終了します。
 *
 * DirectX 11を使用してImGuiの描画データをレンダリングします。
 */
void DebugUI::EndDraw()
{
	// ImGuiの描画データをレンダリング
	ImGui::Render();
	// DirectX 11を使用してImGuiの描画データをレンダリング
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
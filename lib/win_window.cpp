#include "win_window.h"

namespace WIN {
	void Window::CreateWinClass(wchar_t *className, HINSTANCE hInstance) {

		wcex.lpszClassName = className;						// ウィンドウクラス名
		wcex.cbSize = sizeof(WNDCLASSEX);					// ウィンドウクラスのサイズ
		wcex.style = CS_CLASSDC;							// スタイル
		wcex.lpfnWndProc = WndProc;							// ウィンドウプロシージャ
		wcex.hIcon = NULL;									// アイコン
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			// カーソル
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// 背景
		wcex.lpszMenuName = NULL;							// ウィンドウメニューハンドル
		wcex.hInstance = hInstance;							// ハンドルインスタンス
		wcex.cbClsExtra = wcex.cbWndExtra = 0;
		wcex.hIconSm = NULL;

		// ウィンドウクラスの登録
		RegisterClassEx(&wcex);
	}

	void Window::CreateHwnd(wchar_t *className, wchar_t* name, UINT style, HINSTANCE hInstance, HWND hwnd, Vector2Int pos, Vector2Int size) {
		hwnd = CreateWindowEx(
			/*WS_EX_LAYERED*/NULL,			// 描画スタイル（透明化）
			className,						// ウィンドウクラス名
			name,							// ウィンドウ名
			style,							// ウィンドウスタイル
			pos.x, pos.y,					// ウィンドウ座標
			size.x, size.y,					// ウィンドウサイズ
			hwnd,							// 親ウィンドウハンドル
			NULL,							// ウィンドウメニューハンドル
			hInstance,						// インスタンスハンドル
			this							// アプリに追加するデータ
		);
	}

	void Window::Create(const wchar_t *className, const wchar_t *name, HINSTANCE hInstance, Vector2Int pos, Vector2Int size) {
		CreateWinClass((wchar_t *)className, hInstance);
		CreateHwnd((wchar_t *)className, (wchar_t *)name, WS_OVERLAPPEDWINDOW, hInstance, nullptr, pos, size);
	}

	void Window::Create(const wchar_t *className, const wchar_t *name, UINT style, HINSTANCE hInstance, HWND hwnd, Vector2Int pos, Vector2Int size) {
		CreateWinClass((wchar_t *)className, hInstance);
		CreateHwnd((wchar_t *)className, (wchar_t *)name, style, hInstance, hwnd, pos, size);
	}

	Vector2Int Window::GetSize() {
		RECT rect = {};
		Vector2Int size;
		GetWindowRect(hwnd, &rect);
		return { rect.right - rect.left, rect.bottom - rect.top };
	}

	Vector2Int Window::GetPosition() {
		RECT rect = {};
		Vector2Int size;
		GetWindowRect(hwnd, &rect);
		return { rect.left, rect.top };
	}

	Rect Window::GetRect() {
		RECT rect = {};
		GetWindowRect(hwnd, &rect);
		return rect;
	}

	void Window::Show(void) {
		if (hwnd == nullptr) return;

		// 表示
		ShowWindow(hwnd, SW_SHOW);
	}

	LRESULT Window::Proc(UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message)
		{
		case WM_DESTROY:
			isQuit = true;
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
			WIN::Input::keys[wParam] = true;
			return 0;

		case WM_KEYUP:
			WIN::Input::keys[wParam] = false;
			return 0;
		}

		return DefWindowProc(hwnd, message, wParam, lParam);
	}
};
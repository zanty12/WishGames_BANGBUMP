#include "win_window.h"

namespace WIN {
	void Window::CreateWinClass(wchar_t *className, HINSTANCE hInstance) {

		wcex.lpszClassName = className;						// �E�B���h�E�N���X��
		wcex.cbSize = sizeof(WNDCLASSEX);					// �E�B���h�E�N���X�̃T�C�Y
		wcex.style = CS_CLASSDC;							// �X�^�C��
		wcex.lpfnWndProc = WndProc;							// �E�B���h�E�v���V�[�W��
		wcex.hIcon = NULL;									// �A�C�R��
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);			// �J�[�\��
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// �w�i
		wcex.lpszMenuName = NULL;							// �E�B���h�E���j���[�n���h��
		wcex.hInstance = hInstance;							// �n���h���C���X�^���X
		wcex.cbClsExtra = wcex.cbWndExtra = 0;
		wcex.hIconSm = NULL;

		// �E�B���h�E�N���X�̓o�^
		RegisterClassEx(&wcex);
	}

	void Window::CreateHwnd(wchar_t *className, wchar_t* name, UINT style, HINSTANCE hInstance, HWND hwnd, Vector2Int pos, Vector2Int size) {
		hwnd = CreateWindowEx(
			/*WS_EX_LAYERED*/NULL,			// �`��X�^�C���i�������j
			className,						// �E�B���h�E�N���X��
			name,							// �E�B���h�E��
			style,							// �E�B���h�E�X�^�C��
			pos.x, pos.y,					// �E�B���h�E���W
			size.x, size.y,					// �E�B���h�E�T�C�Y
			hwnd,							// �e�E�B���h�E�n���h��
			NULL,							// �E�B���h�E���j���[�n���h��
			hInstance,						// �C���X�^���X�n���h��
			this							// �A�v���ɒǉ�����f�[�^
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

		// �\��
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
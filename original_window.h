#pragma once
#include "lib/winlib.h"
#include "DebugUI.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class OrigialWindow : public WIN::Window {
	LRESULT Proc(UINT message, WPARAM wParam, LPARAM lParam) override {
		if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
			return true;
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
};
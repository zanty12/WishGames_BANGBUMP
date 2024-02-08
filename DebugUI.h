#pragma once

#include "lib/ImGui/imgui.h"
#include "lib/ImGui/imgui_impl_win32.h"
#include "lib/ImGui/imgui_impl_dx11.h"
class DebugUI
{
public:
	static void Initialize(void);
	static void Release(void);
	static void BeginDraw(void);
	static void EndDraw(void);

};

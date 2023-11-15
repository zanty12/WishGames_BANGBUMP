#pragma once
#include "lib/dxlib.h"
#include "lib/winlib.h"
#pragma comment(lib, "lib/lib.lib")

class Graphical {
private:
	static WIN::Window window;
	static DX::DX11::Device3D device;
	static DX::DX11::Renderer renderer;

public:
	static void Initialize(int width, int height);
	static void Release(void);

	static void Clear(Color color);
	static void Present(void);
	static const WIN::Window &GetHwnd(void) { return window; }
};
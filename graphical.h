#pragma once
#include "lib/dxlib.h"
#include "lib/winlib.h"
#include "original_window.h"
#ifdef _DEBUG
#pragma comment(lib, "lib/lib.lib")
#else
#pragma comment(lib, "lib/release_lib.lib")
#endif

class Graphical {
private:
	static OrigialWindow window;
	static DX::DX11::Device3D device;
	static DX::DX11::Renderer renderer;
	static int width;
	static int height;

public:
	static void Initialize(int width, int height);
	static void Release(void);

	static void Clear(Color color);
	static void Present(void);
	static const WIN::Window &GetHwnd(void) { return window; }
	static const DX::DX11::Device3D &GetDevice(void) { return device; }
	static const DX::DX11::Renderer &GetRenderer(void) { return renderer; }
	static int GetWidth(void) { return width; }
	static int GetHeight(void) { return height; }
};
#include "graphical.h"
#include "sprite.h"

WIN::Window Graphical::window;
DX::DX11::Device3D Graphical::device;
DX::DX11::Renderer Graphical::renderer;

void Graphical::Initialize(int width, int height) {
	using namespace DX::DX11;
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	// ウィンドウの作成
	window.Create(L"App", L"alpha", hInstance, { 0,0 }, { width, height });
	// デバイスの作成
	device.Create();
	// レンダラーの作成
	renderer.Create(window, device);


	// レンダラーの更新
	Device3D::SetResource(renderer.GetBlend());
	Device3D::SetResource(renderer.GetDepthstencil());
	Device3D::SetResource(renderer.GetSampler());
	Device3D::SetResource(renderer.GetRasterizer());
	device.Target(renderer);


	// スプライトの初期化
	InitSprite();

	// ウィンドウの表示
	window.Show();
}

void Graphical::Release() {
	ReleaseSprite();
	renderer.Release();
	device.Release();
}

void Graphical::Clear(Color color) {
	using namespace DX::DX11;
	renderer.Clear(color, Device3D::GetContext());
}

void Graphical::Present(void) {
	renderer.Present();
}

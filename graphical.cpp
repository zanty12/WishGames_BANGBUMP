#include "graphical.h"
#include "sprite.h"
#ifdef _DEBUG
#pragma comment(lib, "lib/debug_lib.lib")
#else
#pragma comment(lib, "lib/release_lib.lib")
#endif

OrigialWindow Graphical::window;
DX::DX11::Device3D Graphical::device;
DX::DX11::Renderer Graphical::renderer;
int Graphical::width = 0;
int Graphical::height = 0;

void Graphical::Initialize(int width, int height) {
	using namespace DX::DX11;
	Graphical::width = width;
	Graphical::height = height;

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	// �E�B���h�E�̍쐬
	window.Create(L"App", L"alpha", hInstance, { 0,0 }, { width, height });
	// �f�o�C�X�̍쐬
	device.Create();
	// �����_���[�̍쐬
	renderer.Create(window, device);


	// �����_���[�̍X�V
	Device3D::SetResource(renderer.GetBlend());
	Device3D::SetResource(renderer.GetDepthstencil());
	Device3D::SetResource(renderer.GetSampler());
	Device3D::SetResource(renderer.GetRasterizer());
	device.Target(renderer);

	Graphical::WindowShow();


	// �X�v���C�g�̏�����
	InitSprite();
}

void Graphical::WindowShow(void) {
	// �E�B���h�E�̕\��
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

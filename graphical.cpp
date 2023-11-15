#include "graphical.h"
#include "sprite.h"

WIN::Window Graphical::window;
DX::DX11::Device3D Graphical::device;
DX::DX11::Renderer Graphical::renderer;

void Graphical::Initialize(int width, int height) {
	using namespace DX::DX11;
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


	// �X�v���C�g�̏�����
	InitSprite();

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

#include "dx11_swapchain.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			DXGI_SWAP_CHAIN_DESC SWAPCHAIN::Desc(HWND hwnd, BOOL windowed)
			{
				DXGI_SWAP_CHAIN_DESC desc;
				RECT rect;												// ���N�g
				int width;												// ��
				int height;												// ����
				// Rect���擾���ĕ��ƍ��������߂�
				GetClientRect(hwnd, &rect);
				width = rect.right - rect.left;
				height = rect.bottom - rect.top;

				// SwapChain
				ZeroMemory(&desc, sizeof(desc));
				desc.BufferCount = 2;									// �o�b�t�@��
				desc.BufferDesc.Width = width;							// �X���b�v�`�F�[���̕�
				desc.BufferDesc.Height = height;						// �X���b�v�`�F�[���̍���
				desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// �t�H�[�}�b�g
				desc.BufferDesc.RefreshRate.Numerator = 60;				// 1�t���[��������̏����iDenominator / Numerator�j
				desc.BufferDesc.RefreshRate.Denominator = 1;
				desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// �g�p���@�i�o�b�t�@�������_�\�^�[�Q�b�g�Ƃ��Ďg�p�j
				// MSAA�̐ݒ�
				desc.SampleDesc.Count = 1;								// �}���`�T���v�����O�̃s�N�Z���P�ʂ̐�
				desc.SampleDesc.Quality = 0;							// �}���`�T���v�����O�̕i���i�A���`�G�C���A�X�j
				// �E�B���h�E�̐ݒ� 
				desc.Windowed = windowed;								// �E�B���h�E���[�h
				desc.OutputWindow = hwnd;								// �o�͂���n���h���E�B���h�E

				return desc;
			}

			HRESULT SWAPCHAIN::Create(DXGI_SWAP_CHAIN_DESC desc, ID3D11Device *device) {
				NULL_CHECK(device);
				CREATE_CHECK(resource);

				HRESULT hr;
				IDXGIDevice1 *dxgi = NULL;
				IDXGIAdapter *adapter = NULL;
				IDXGIFactory *factory = NULL;
				this->desc = desc;

				// DXGI -> Adapter -> Factory �Ǝ擾����
				// �X���b�v�`�F�[�����쐬����
				hr = device->QueryInterface(IID_PPV_ARGS(&dxgi));
				ERROR_CHECK(hr, "Error create dxgi");
				hr = dxgi->GetAdapter(&adapter);
				ERROR_CHECK(hr, "Error create adapter");
				hr = adapter->GetParent(IID_PPV_ARGS(&factory));
				ERROR_CHECK(hr, "Error create factory.");
				hr = factory->CreateSwapChain(device, &desc, &resource);
				ERROR_CHECK(hr, "Error create swapchain.");

				// �������̊J��
				dxgi->Release();
				adapter->Release();
				factory->Release();

				return hr;
			}
		}
	}
}

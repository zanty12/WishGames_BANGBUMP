#include "dx11_rtv.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			D3D11_RENDER_TARGET_VIEW_DESC RTV::Desc(DXGI_SWAP_CHAIN_DESC scDesc) {
				D3D11_RENDER_TARGET_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Format = scDesc.BufferDesc.Format;
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

				return desc;
			}

			HRESULT RTV::Create(D3D11_RENDER_TARGET_VIEW_DESC desc, IDXGISwapChain *swapchain, ID3D11Device *device) {
				NULL_CHECK(device);
				NULL_CHECK(swapchain);
				CREATE_CHECK(resource);

				// RenderTargetView�̍쐬
				HRESULT hr;
				ID3D11Texture2D *backBuffer = NULL;
				// �o�b�N�o�b�t�@�̎擾
				hr = swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
				ERROR_CHECK(hr, "Error get backbuffer.");
				// RenderTargetView�̍쐬
				hr = device->CreateRenderTargetView(backBuffer, &desc, &resource);

				// �������̊J��
				backBuffer->Release();
			}
		}
	}
}

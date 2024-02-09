#include "dx11_depthstencilview.h"
#include "dx11_texture.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			CD3D11_DEPTH_STENCIL_VIEW_DESC DEPTHSTENCILVIEW::Desc()
			{
				CD3D11_DEPTH_STENCIL_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Format = desc.Format;								// �t�H�[�}�b�g
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;		// 2D�e�N�X�`�����g�p
				desc.Flags = 0;											// �t���O

				return desc;
			}

			HRESULT DEPTHSTENCILVIEW::Create(CD3D11_DEPTH_STENCIL_VIEW_DESC desc, DXGI_SWAP_CHAIN_DESC scDesc, ID3D11Device *device) {
				NULL_CHECK(device);
				CREATE_CHECK(resource);
				HRESULT hr;

				// �[�x�e�N�X�`���̍쐬
				hr = depthTexture.Create(TEXTURE::DescDepthStencil(scDesc), device);
				ERROR_CHECK(hr, "Error create depthtexture.");

				// �[�x�X�e���V���r���[�̍쐬
				hr = device->CreateDepthStencilView(depthTexture, &desc, &resource);
				ERROR_CHECK(hr, "Error create depthstencilview.");

				return hr;
			}
		}
	};
};

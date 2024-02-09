#include "dx11_depthstencilview.h"
#include "dx11_texture.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			CD3D11_DEPTH_STENCIL_VIEW_DESC DEPTHSTENCILVIEW::Desc()
			{
				CD3D11_DEPTH_STENCIL_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Format = desc.Format;								// フォーマット
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;		// 2Dテクスチャを使用
				desc.Flags = 0;											// フラグ

				return desc;
			}

			HRESULT DEPTHSTENCILVIEW::Create(CD3D11_DEPTH_STENCIL_VIEW_DESC desc, DXGI_SWAP_CHAIN_DESC scDesc, ID3D11Device *device) {
				NULL_CHECK(device);
				CREATE_CHECK(resource);
				HRESULT hr;

				// 深度テクスチャの作成
				hr = depthTexture.Create(TEXTURE::DescDepthStencil(scDesc), device);
				ERROR_CHECK(hr, "Error create depthtexture.");

				// 深度ステンシルビューの作成
				hr = device->CreateDepthStencilView(depthTexture, &desc, &resource);
				ERROR_CHECK(hr, "Error create depthstencilview.");

				return hr;
			}
		}
	};
};

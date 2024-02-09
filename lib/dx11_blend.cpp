#include "dx11_blend.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			D3D11_BLEND_DESC BLEND::Desc() {
				D3D11_BLEND_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.AlphaToCoverageEnable = FALSE;
				desc.IndependentBlendEnable = FALSE;
				desc.RenderTarget[0].BlendEnable = TRUE;
				desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
				desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

				return desc;
			}

			HRESULT BLEND::Create(D3D11_BLEND_DESC desc, ID3D11Device *device) {
				NULL_CHECK(device);
				CREATE_CHECK(resource);
				HRESULT hr;

				hr = device->CreateBlendState(&desc, &resource);
				return hr;
			}
		}
	}
};

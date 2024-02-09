#include "dx11_depthstencil.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			D3D11_DEPTH_STENCIL_DESC DEPTHSTENCIL::Desc() {
				D3D11_DEPTH_STENCIL_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.DepthEnable = TRUE;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				desc.DepthFunc = D3D11_COMPARISON_LESS;
				desc.StencilEnable = FALSE;

				return desc;
			}

			HRESULT DEPTHSTENCIL::Create(D3D11_DEPTH_STENCIL_DESC desc, ID3D11Device *device) {
				NULL_CHECK(device);
				CREATE_CHECK(resource);
				HRESULT hr;

				hr = device->CreateDepthStencilState(&desc, &resource);
				return hr;
			}
		}
	}
};

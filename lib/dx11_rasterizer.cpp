#include "dx11_rasterizer.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			D3D11_RASTERIZER_DESC RASTERIZER::Desc() {
				D3D11_RASTERIZER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.FillMode = D3D11_FILL_SOLID;		// “h‚è‚Â‚Ô‚µƒ‚[ƒh
				desc.CullMode = D3D11_CULL_FRONT;		// ƒ|ƒŠƒSƒ“‚Ì•\— ‚Ì•`‰æİ’è
				desc.FrontCounterClockwise = FALSE;		// ”½Œv‰ñ‚è‚ª•\
				desc.DepthClipEnable = TRUE;
				desc.MultisampleEnable = FALSE;

				return desc;
			}

			D3D11_RASTERIZER_DESC RASTERIZER::Desc(D3D11_FILL_MODE fill, D3D11_CULL_MODE cull) {
				D3D11_RASTERIZER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.FillMode = fill;					// “h‚è‚Â‚Ô‚µƒ‚[ƒh
				desc.CullMode = cull;					// ƒ|ƒŠƒSƒ“‚Ì•\— ‚Ì•`‰æİ’è
				desc.FrontCounterClockwise = FALSE;		// ”½Œv‰ñ‚è‚ª•\
				desc.DepthClipEnable = TRUE;
				desc.MultisampleEnable = FALSE;

				return desc;
			}

			HRESULT RASTERIZER::Create(D3D11_RASTERIZER_DESC desc, ID3D11Device *device) {
				NULL_CHECK(device);
				CREATE_CHECK(resource);
				HRESULT hr;

				hr = device->CreateRasterizerState(&desc, &resource);
				return hr;
			}
		}
	}
};

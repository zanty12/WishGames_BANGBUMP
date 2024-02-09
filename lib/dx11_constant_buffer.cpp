#include "dx11_constant_buffer.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			HRESULT ConstantBuffer::Create(UINT structSize, ID3D11Device *device) {
				NULL_CHECK(device);
				HRESULT hr;

				// バッファの作成
				hr = Buffer::Create(Buffer::ConstantDesc(structSize, 0), nullptr, device);
				return hr;
			}
		}
	}
};

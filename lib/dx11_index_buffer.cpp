#include "dx11_index_buffer.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			HRESULT IndexBuffer::Create(DWORD *index, UINT indexNum, PRIMITIVE_SET_TYPE setType, ID3D11Device *device) {
				NULL_CHECK(device);
				HRESULT hr;

				// サブリソース
				D3D11_SUBRESOURCE_DATA desc;
				desc.pSysMem = index;
				desc.SysMemPitch = sizeof(DWORD);
				desc.SysMemSlicePitch = 0;

				// バッファの作成
				UINT indexSize = sizeof(DWORD) * indexNum;
				hr = Buffer::Create(Buffer::IndexDesc(indexSize), &desc, device);
				ERROR_CHECK(hr, "Error create buffer.");

				// リソースの作成
				hr = PrimitiveBuffer<DWORD>::CreateResource(index, indexNum, setType);

				return hr;
			}
		}
	}
};

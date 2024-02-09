#include "dx11_index_buffer.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			HRESULT IndexBuffer::Create(DWORD *index, UINT indexNum, PRIMITIVE_SET_TYPE setType, ID3D11Device *device) {
				NULL_CHECK(device);
				HRESULT hr;

				// �T�u���\�[�X
				D3D11_SUBRESOURCE_DATA desc;
				desc.pSysMem = index;
				desc.SysMemPitch = sizeof(DWORD);
				desc.SysMemSlicePitch = 0;

				// �o�b�t�@�̍쐬
				UINT indexSize = sizeof(DWORD) * indexNum;
				hr = Buffer::Create(Buffer::IndexDesc(indexSize), &desc, device);
				ERROR_CHECK(hr, "Error create buffer.");

				// ���\�[�X�̍쐬
				hr = PrimitiveBuffer<DWORD>::CreateResource(index, indexNum, setType);

				return hr;
			}
		}
	}
};

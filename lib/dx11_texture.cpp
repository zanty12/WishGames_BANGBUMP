#include "dx11_texture.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			D3D11_TEXTURE2D_DESC TEXTURE::DescDepthStencil(DXGI_SWAP_CHAIN_DESC scDesc) {
				D3D11_TEXTURE2D_DESC desc;
				ZeroMemory(&desc, sizeof(desc));				// ������
				desc.Width = scDesc.BufferDesc.Width;			// �o�b�t�@�̕�
				desc.Height = scDesc.BufferDesc.Height;			// �o�b�t�@�̍���
				desc.MipLevels = 1;								// �~�b�v���x��
				desc.ArraySize = 1;								// �z��̃T�C�Y
				desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// �t�H�[�}�b�g
				desc.Usage = D3D11_USAGE_DEFAULT;				// �o�b�t�@�̎g�p���@�i�������ݕs�j
				desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// �o�b�t�@�̎�ށi�f�v�X�X�e���V���p�Ɏw��j
				desc.CPUAccessFlags = 0;						// CPU�ɃA�N�Z�X���邩�ۂ��iCPU�A�N�Z�X�s�\�j
				desc.MiscFlags = 0;
				// MSAA�̐ݒ�
				desc.SampleDesc.Count = 1;						// �s�N�Z���̎擾��
				desc.SampleDesc.Quality = 0;					// �N�I���e�B
				return desc;
			}

			HRESULT TEXTURE::Create(D3D11_TEXTURE2D_DESC desc, ID3D11Device *device) {
				NULL_CHECK(device);
				CREATE_CHECK(resource);
				HRESULT hr;

				hr = device->CreateTexture2D(&desc, NULL, &resource);
				return hr;
			}
		}
	}
}

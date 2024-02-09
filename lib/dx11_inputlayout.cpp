#include "dx11_inputlayout.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			D3D11_INPUT_ELEMENT_DESC INPUTLAYOUT::Desc(const char *name, UINT slot, INPUT_LAYOUT_FORMAT format) {
				// https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_input_element_desc
				D3D11_INPUT_ELEMENT_DESC desc;

				desc.SemanticName = name;															// ���O
				desc.SemanticIndex = 0;																// �Z�}���e�B�b�N�C���f�b�N�X
				switch (format) {																	// �t�H�[�}�b�g
				case FORMAT_FLOAT1: desc.Format = DXGI_FORMAT_R32_FLOAT; break;
				case FORMAT_FLOAT2: desc.Format = DXGI_FORMAT_R32G32_FLOAT; break;
				case FORMAT_FLOAT3: desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
				case FORMAT_FLOAT4: desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
				}
				desc.InputSlot = slot;																// ���_�o�b�t�@�̃X���b�g
				desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;								// �I�t�Z�b�g
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;									// �X���b�g�̓��̓N���X
				desc.InstanceDataStepRate = 0;														// ���_�f�[�^�̍ė��p��

				return desc;
			}

			D3D11_INPUT_ELEMENT_DESC INPUTLAYOUT::Desc(const char *name, UINT slot, UINT startBytePosition, INPUT_LAYOUT_FORMAT format) {
				// https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_input_element_desc
				D3D11_INPUT_ELEMENT_DESC desc;

				desc.SemanticName = name;															// ���O
				desc.SemanticIndex = 0;																// �Z�}���e�B�b�N�C���f�b�N�X
				switch (format) {																	// �t�H�[�}�b�g
				case FORMAT_FLOAT1: desc.Format = DXGI_FORMAT_R32_FLOAT; break;
				case FORMAT_FLOAT2: desc.Format = DXGI_FORMAT_R32G32_FLOAT; break;
				case FORMAT_FLOAT3: desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
				case FORMAT_FLOAT4: desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
				}
				desc.InputSlot = slot;																// ���_�o�b�t�@�̃X���b�g
				desc.AlignedByteOffset = startBytePosition;											// �I�t�Z�b�g
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;									// �X���b�g�̓��̓N���X
				desc.InstanceDataStepRate = 0;														// ���_�f�[�^�̍ė��p��

				return desc;
			}

			HRESULT INPUTLAYOUT::Create(D3D11_INPUT_ELEMENT_DESC desc[], UINT descNum, ID3DBlob *blob, ID3D11Device *device) {
				NULL_CHECK(desc);
				NULL_CHECK(blob);
				NULL_CHECK(device);
				CREATE_CHECK(resource);
				HRESULT hr;

				hr = device->CreateInputLayout(
					desc,																			// �f�[�^�\���̏��
					descNum,																		// desc�̗v�f��
					blob->GetBufferPointer(),														// ���_�V�F�[�_�̃R���p�C�����ꂽ�f�[�^�̃|�C���^
					blob->GetBufferSize(),															// ���_�V�F�[�_�̃R���p�C�����ꂽ�f�[�^�̃T�C�Y
					&resource
				);

				return hr;
			}
		}
	}
}

/*==============================================================================

	Title		: INPUTLAYOUT
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_INPUTLAYOUT_H_
#define _DX11_INPUTLAYOUT_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include <d3d11.h>
#include "dx_dxparts.h"

/*------------------------------------------------------------------------------
* define�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class�錾
------------------------------------------------------------------------------*/
namespace DX {
	namespace DX11 {
		namespace CORE {
			enum INPUT_LAYOUT_FORMAT {
				FORMAT_FLOAT1,
				FORMAT_FLOAT2,
				FORMAT_FLOAT3,
				FORMAT_FLOAT4,
			};

			class INPUTLAYOUT : public DX::CORE::DX_PARTS<ID3D11InputLayout, D3D11_INPUT_ELEMENT_DESC[], ID3D11Device> {
			public:

				/// <summary>
				/// �f�t�H���g�f�X�N
				/// </summary>
				/// <param name="name">���O</param>
				/// <param name="slot">�X���b�g</param>
				/// <param name="format">�t�H�[�}�b�g</param>
				/// <returns></returns>
				static D3D11_INPUT_ELEMENT_DESC Desc(const char *name, UINT slot, INPUT_LAYOUT_FORMAT format);

				/// <summary>
				/// �f�t�H���g�f�X�N
				/// </summary>
				/// <param name="name">���O</param>
				/// <param name="slot">�X���b�g</param>
				/// <param name="startBytePosition">�J�n�ʒu</param>
				/// <param name="format">�t�H�[�}�b�g</param>
				/// <returns></returns>
				static D3D11_INPUT_ELEMENT_DESC Desc(const char *name, UINT slot, UINT startBytePosition, INPUT_LAYOUT_FORMAT format);

				/// <summary>
				/// �C���v�b�g���C�A�E�g�̍쐬
				/// </summary>
				/// <param name="desc"�f�X�N���</param>
				/// <param name="descNum">�f�X�N�̔z��̌�</param>
				/// <param name="blob">�u���b�u</param>
				/// <param name="device">�f�o�C�X�|�C���^</param>
				/// <returns></returns>
				HRESULT Create(D3D11_INPUT_ELEMENT_DESC desc[], UINT descNum, ID3DBlob *blob, ID3D11Device *device);
			};
		}
	}
}

#endif

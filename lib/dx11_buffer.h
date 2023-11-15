/*==============================================================================

	Title		: BUFFER
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_BUFFER_H_
#define _DX11_BUFFER_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include <d3d11.h>
#include <DirectXMath.h>
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
			class Buffer : public DX::CORE::DX_PARTS<ID3D11Buffer, D3D11_BUFFER_DESC, ID3D11Device> {
			public:

				/// <summary>
				/// �f�t�H���g�f�X�N�̎擾
				/// </summary>
				/// <param name="structSize">�\���̂̃T�C�Y</param>
				/// <param name="usage">�g�p���@�iGPU�̂݃A�N�Z�X�� or CPU�ł��A�N�Z�X�� etc.�j</param>
				/// <param name="bindFlag">�o�b�t�@�[�̖���</param>
				/// <param name="cpuAccessFlag">CPU�ւ̃A�N�Z�X�t���O</param>
				/// <param name="stride">�\���̂̃X�L�b�v</param>
				/// <returns></returns>
				static D3D11_BUFFER_DESC Desc(UINT structSize, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlag, UINT cpuAccessFlag, UINT stride);

				/// <summary>
				/// �萔�o�b�t�@�f�X�N�̎擾
				/// </summary>
				/// <param name="structSize">�\���̂̃T�C�Y</param>
				/// <param name="stride">�\���̂̃X�L�b�v</param>
				/// <returns></returns>
				static D3D11_BUFFER_DESC ConstantDesc(UINT structSize, UINT stride);

				/// <summary>
				/// ���_�o�b�t�@�f�X�N�̎擾
				/// </summary>
				/// <returns></returns>
				static D3D11_BUFFER_DESC VertexDesc(UINT vertexSize);

				/// <summary>
				/// �C���f�b�N�X�o�b�t�@�f�X�N�̎擾
				/// </summary>
				/// <returns></returns>
				static D3D11_BUFFER_DESC IndexDesc(UINT indexSize);

				/// <summary>
				/// �o�b�t�@�̍쐬
				/// </summary>
				/// <param name="desc">�f�X�N���</param>
				/// <param name="device">�f�o�C�X�|�C���^</param>
				/// <returns></returns>
				HRESULT Create(D3D11_BUFFER_DESC desc, ID3D11Device *device) override;

				/// <summary>
				/// �o�b�t�@�̍쐬
				/// </summary>
				/// <param name="desc">�f�X�N���</param>
				/// <param name="resourceDesc">���\�[�X�f�X�N���</param>
				/// <param name="device">�f�o�C�X�|�C���^</param>
				/// <returns></returns>
				HRESULT Create(D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA *resourceDesc, ID3D11Device *device);
			};
		}
	}
}

#endif

/*==============================================================================

	Title		: DEVICE3D
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_DEVICE3D_H_
#define _DX11_DEVICE3D_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include <d3d11.h>
#include "dx_device_model.h"
#include "dx11_renderer.h"
#pragma comment(lib, "d3d11.lib")

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
		class Device3D : public DEVICE_MODEL<ID3D11Device, ID3D11DeviceContext> {
		public:

			/// <summary>
			/// �f�o�C�X3D�̍쐬
			/// </summary>
			/// <returns></returns>
			HRESULT Create() override;

			/// <summary>
			/// �^�[�Q�b�g����
			/// </summary>
			/// <param name="renderer">�����_���[</param>
			void Target(Renderer &renderer);

			/// <summary>
			/// �o�b�t�@�[�̐ݒ�
			/// </summary>
			/// <param name="slot">�X���b�g�ԍ�</param>
			/// <param name="buffNum">�o�b�t�@��</param>
			/// <param name="buffer">�o�b�t�@</param>
			static HRESULT SetConstantBuffer(UINT slot, UINT buffNum, ID3D11Buffer *buffer);

			/// <summary>
			/// �o�b�t�@�[�̍X�V
			/// </summary>
			/// <param name="src">�\�[�X</param>
			static HRESULT UpdateConstantBuffer(void *src, ID3D11Buffer* buffer);

			/// <summary>
			/// �T���v���[�̐ݒ�
			/// </summary>
			/// <param name="sampler">�T���v���[</param>
			static HRESULT SetResource(ID3D11SamplerState *sampler);

			/// <summary>
			/// ���X�^���C�U�[�̐ݒ�
			/// </summary>
			/// <param name="rasterizer">���X�^���C�U�[</param>
			static HRESULT SetResource(ID3D11RasterizerState *rasterizer);

			/// <summary>
			/// �u�����h�̐ݒ�
			/// </summary>
			/// <param name="blend">�u�����h</param>
			static HRESULT SetResource(ID3D11BlendState *blend);

			/// <summary>
			/// �[�x�X�e���V���̐ݒ�
			/// </summary>
			/// <param name="depthstencil">�[�x�X�e���V��</param>
			static HRESULT SetResource(ID3D11DepthStencilState *depthstencil);

			/// <summary>
			/// ���\�[�X�̐ݒ�
			/// </summary>
			/// <param name="resource">���\�[�X</param>
			static HRESULT SetResource(ID3D11ShaderResourceView *resource);

			/// <summary>
			/// ���\�[�X�̐ݒ�
			/// </summary>
			/// <param name="shader">���_�V�F�[�_�[</param>
			/// <param name="inputLayout">���̓��C�A�E�g</param>
			static HRESULT SetResource(ID3D11VertexShader *shader, ID3D11InputLayout *inputLayout);

			/// <summary>
			/// ���\�[�X�̐ݒ�
			/// </summary>
			/// <param name="shader">�s�N�Z���V�F�[�_�[</param>
			static HRESULT SetResource(ID3D11PixelShader *shader);

			/// <summary>
			/// �o�b�t�@�ɏ�������
			/// </summary>
			/// <param name="vertex">���_�o�b�t�@</param>
			/// <param name="vertexNum">���_��</param>
			/// <param name="vertexSize">���_�o�b�t�@�̃T�C�Y</param>
			/// <param name="index">�C���f�b�N�X�o�b�t�@</param>
			/// <param name="indexNum">�C���f�b�N�X��</param>
			/// <param name="indexSize">�C���f�b�N�X�o�b�t�@�̃T�C�Y</param>
			/// <param name="deviceContext">�f�o�C�X�R���e�L�X�g</param>
			static HRESULT Draw(ID3D11Buffer *vertex, UINT vertexNum, UINT vertexStructSize, ID3D11Buffer *index, UINT indexNum, UINT indexStructSize);

			/// <summary>
			/// �g�|���W�[�̐ݒ�
			/// </summary>
			/// <param name="topology">�g�|���W�[</param>
			/// <param name="deviceContext"></param>
			static void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
		};
	}
}

#endif

/*==============================================================================

	Title		: GRAPHICAL
	Description : �O���t�B�J��
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_GRAPHICAL_H_
#define _DX11_GRAPHICAL_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include <d3d11.h>
// �`��
#include "dx11_swapchain.h"
#include "dx11_viewport.h"
#include "dx11_depthstencilview.h"
#include "dx11_rtv.h"
// �X�e�[�g
#include "dx11_blend.h"
#include "dx11_depthstencil.h"
#include "dx11_rasterizer.h"
#include "dx11_sampler.h"
// �V�F�[�_�[
#include "dx11_vertex_shader.h"
#include "dx11_pixel_shader.h"
#include "dx11_inputlayout.h"
// �o�b�t�@
#include "dx11_constant_buffer.h"
#include "dx_matrix.h"

#include "stdlib.h"

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
		class Renderer {
		private:
			CORE::VIEWPORT viewport;				// �r���[�|�[�g
			
			CORE::SWAPCHAIN swapchain;				// �X���b�v�`�F�[��
			CORE::DEPTHSTENCILVIEW depthstencilview;// �[�x�X�e���V��
			CORE::RTV rtv;							// �����_�\�^�[�Q�b�g�r���[

			CORE::BLEND blend;						// �u�����h
			CORE::DEPTHSTENCIL depthstencil;		// �f�v�X�X�e���V��
			CORE::RASTERIZER rasterizer;			// ���X�^���C�Y
			CORE::SAMPLER sampler;					// �T���v���\



		public:
			~Renderer() { Release(); }

			/// <summary>
			/// �X���b�v�`�F�[���ƃ����_�[�^�[�Q�b�g�r���[�̍쐬
			/// </summary>
			/// <param name="hwnd">�n���h���E�B���h�E</param>
			/// <param name="device3d">�f�o�C�X3D</param>
			/// <returns></returns>
			HRESULT Create(HWND hwnd, BOOL windowed, DXGI_FORMAT format, ID3D11Device *device);

			CORE::VIEWPORT &GetViewport() { return viewport; }
			CORE::SWAPCHAIN &GetSwapchain() { return swapchain; }
			CORE::DEPTHSTENCILVIEW &GetDepthstencilview() { return depthstencilview; }
			CORE::RTV &GetRTV() { return rtv; }

			CORE::BLEND &GetBlend() { return blend; }
			CORE::DEPTHSTENCIL &GetDepthstencil() { return depthstencil; }
			CORE::RASTERIZER &GetRasterizer() { return rasterizer; }
			CORE::SAMPLER &GetSampler() { return sampler; }

			/// <summary>
			/// ���_�V�F�[�_���Z�b�g����
			/// </summary>
			/// <param name="shader">���_�V�F�[�_</param>
			/// <param name="inputLayout">�C���v�b�g���C�A�E�g</param>
			/// <param name="deviceContext">�f�o�C�X�R���e�L�X�g</param>
			/// <returns></returns>
			HRESULT SetShader(ID3D11VertexShader *shader, ID3D11InputLayout *inputLayout, ID3D11DeviceContext* deviceContext);

			/// <summary>
			/// �s�N�Z���V�F�[�_���Z�b�g����
			/// </summary>
			/// <param name="shader">�s�N�Z���V�F�[�_</param>
			/// <param name="deviceContext">�f�o�C�X�R���e�L�X�g</param>
			/// <returns></returns>
			HRESULT SetShader(ID3D11PixelShader *shader, ID3D11DeviceContext* deviceContext);

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
			HRESULT Draw(ID3D11Buffer *vertex, UINT vertexNum, UINT vertexStructSize, ID3D11Buffer *index, UINT indexNum, UINT indexStructSize, ID3D11DeviceContext *deviceContext);
			
			/// <summary>
			/// ��ʂ̃N���A
			/// </summary>
			/// <param name="color">�N���A�ɂ���F</param>
			/// <param name="deviceContext">�f�o�C�X�R���e�L�X�g</param>
			HRESULT Clear(Color color, ID3D11DeviceContext *deviceContext);

			/// <summary>
			/// �v���~�e�B�u�g�|���W�[�̐ݒ�
			/// </summary>
			/// <param name="topology">�v���~�e�B�u�g�|���W�[</param>
			/// <param name="deviceContext">�f�o�C�X�R���e�L�X�g</param>
			HRESULT SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology, ID3D11DeviceContext *deviceContext);

			/// <summary>
			/// �`�悷��
			/// </summary>
			void Present(void);

			/// <summary>
			/// ���
			/// </summary>
			/// <param name=""></param>
			void Release(void);
		};
	}
}

#endif

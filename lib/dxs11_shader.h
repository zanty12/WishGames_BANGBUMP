/*==============================================================================

	Title		: SHADER
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX_GRAPHICAL_SHADER_H_
#define _DX_GRAPHICAL_SHADER_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include "dx11_vertex_shader.h"
#include "dx11_pixel_shader.h"
#include "dx11_inputlayout.h"
#include "dx11_device3d.h"

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
* enum�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class�錾
------------------------------------------------------------------------------*/
namespace DX {
	namespace DX11 {
		namespace GRAPHICAL {
			struct SHADER_DESC {
				const wchar_t *vertexPath;					// ���_�V�F�[�_�\�̃p�X
				const wchar_t *pixelPath;					// �s�N�Z���V�F�[�_�\�̃p�X
				const char *vertexEntryPoint;				// ���_�V�F�[�_�\�̃G���g���[�|�C���g
				const char *pixelEntryPoint;				// �s�N�Z���V�F�[�_�\�̃G���g���[�|�C���g
				D3D11_INPUT_ELEMENT_DESC *desc;				// ���̓��C�A�E�g�̏��
				UINT descNum;								// ���̓��C�A�E�g�̏��̗v�f��
			};



			class Shader {
			private:
				DX::DX11::CORE::VERTEX_SHADER vertexShader;
				DX::DX11::CORE::PIXEL_SHADER pixelShader;
				DX::DX11::CORE::INPUTLAYOUT inputLayout;



			public:
				~Shader() { Release(); }

				/// <summary>
				/// �V�F�[�_�[�쐬
				/// </summary>
				/// <param name="desc">�f�X�N</param>
				/// <returns></returns>
				HRESULT Create(SHADER_DESC desc);

				/// <summary>
				/// �X�V
				/// </summary>
				HRESULT Update(void);

				/// <summary>
				/// ���
				/// </summary>
				void Release(void);
			};
		}
	}
}

#endif

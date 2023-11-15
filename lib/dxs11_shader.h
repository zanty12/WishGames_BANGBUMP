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
* include宣言
------------------------------------------------------------------------------*/
#include "dx11_vertex_shader.h"
#include "dx11_pixel_shader.h"
#include "dx11_inputlayout.h"
#include "dx11_device3d.h"

/*------------------------------------------------------------------------------
* define宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* enum宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class宣言
------------------------------------------------------------------------------*/
namespace DX {
	namespace DX11 {
		namespace GRAPHICAL {
			struct SHADER_DESC {
				const wchar_t *vertexPath;					// 頂点シェーダ―のパス
				const wchar_t *pixelPath;					// ピクセルシェーダ―のパス
				const char *vertexEntryPoint;				// 頂点シェーダ―のエントリーポイント
				const char *pixelEntryPoint;				// ピクセルシェーダ―のエントリーポイント
				D3D11_INPUT_ELEMENT_DESC *desc;				// 入力レイアウトの情報
				UINT descNum;								// 入力レイアウトの情報の要素数
			};



			class Shader {
			private:
				DX::DX11::CORE::VERTEX_SHADER vertexShader;
				DX::DX11::CORE::PIXEL_SHADER pixelShader;
				DX::DX11::CORE::INPUTLAYOUT inputLayout;



			public:
				~Shader() { Release(); }

				/// <summary>
				/// シェーダー作成
				/// </summary>
				/// <param name="desc">デスク</param>
				/// <returns></returns>
				HRESULT Create(SHADER_DESC desc);

				/// <summary>
				/// 更新
				/// </summary>
				HRESULT Update(void);

				/// <summary>
				/// 解放
				/// </summary>
				void Release(void);
			};
		}
	}
}

#endif

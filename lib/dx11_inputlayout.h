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
* include宣言
------------------------------------------------------------------------------*/
#include <d3d11.h>
#include "dx_dxparts.h"

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
* class宣言
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
				/// デフォルトデスク
				/// </summary>
				/// <param name="name">名前</param>
				/// <param name="slot">スロット</param>
				/// <param name="format">フォーマット</param>
				/// <returns></returns>
				static D3D11_INPUT_ELEMENT_DESC Desc(const char *name, UINT slot, INPUT_LAYOUT_FORMAT format);

				/// <summary>
				/// デフォルトデスク
				/// </summary>
				/// <param name="name">名前</param>
				/// <param name="slot">スロット</param>
				/// <param name="startBytePosition">開始位置</param>
				/// <param name="format">フォーマット</param>
				/// <returns></returns>
				static D3D11_INPUT_ELEMENT_DESC Desc(const char *name, UINT slot, UINT startBytePosition, INPUT_LAYOUT_FORMAT format);

				/// <summary>
				/// インプットレイアウトの作成
				/// </summary>
				/// <param name="desc"デスク情報</param>
				/// <param name="descNum">デスクの配列の個数</param>
				/// <param name="blob">ブロッブ</param>
				/// <param name="device">デバイスポインタ</param>
				/// <returns></returns>
				HRESULT Create(D3D11_INPUT_ELEMENT_DESC desc[], UINT descNum, ID3DBlob *blob, ID3D11Device *device);
			};
		}
	}
}

#endif

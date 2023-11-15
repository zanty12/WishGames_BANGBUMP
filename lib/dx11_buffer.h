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
* include宣言
------------------------------------------------------------------------------*/
#include <d3d11.h>
#include <DirectXMath.h>
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
			class Buffer : public DX::CORE::DX_PARTS<ID3D11Buffer, D3D11_BUFFER_DESC, ID3D11Device> {
			public:

				/// <summary>
				/// デフォルトデスクの取得
				/// </summary>
				/// <param name="structSize">構造体のサイズ</param>
				/// <param name="usage">使用方法（GPUのみアクセス可 or CPUでもアクセス可 etc.）</param>
				/// <param name="bindFlag">バッファーの役割</param>
				/// <param name="cpuAccessFlag">CPUへのアクセスフラグ</param>
				/// <param name="stride">構造体のスキップ</param>
				/// <returns></returns>
				static D3D11_BUFFER_DESC Desc(UINT structSize, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlag, UINT cpuAccessFlag, UINT stride);

				/// <summary>
				/// 定数バッファデスクの取得
				/// </summary>
				/// <param name="structSize">構造体のサイズ</param>
				/// <param name="stride">構造体のスキップ</param>
				/// <returns></returns>
				static D3D11_BUFFER_DESC ConstantDesc(UINT structSize, UINT stride);

				/// <summary>
				/// 頂点バッファデスクの取得
				/// </summary>
				/// <returns></returns>
				static D3D11_BUFFER_DESC VertexDesc(UINT vertexSize);

				/// <summary>
				/// インデックスバッファデスクの取得
				/// </summary>
				/// <returns></returns>
				static D3D11_BUFFER_DESC IndexDesc(UINT indexSize);

				/// <summary>
				/// バッファの作成
				/// </summary>
				/// <param name="desc">デスク情報</param>
				/// <param name="device">デバイスポインタ</param>
				/// <returns></returns>
				HRESULT Create(D3D11_BUFFER_DESC desc, ID3D11Device *device) override;

				/// <summary>
				/// バッファの作成
				/// </summary>
				/// <param name="desc">デスク情報</param>
				/// <param name="resourceDesc">リソースデスク情報</param>
				/// <param name="device">デバイスポインタ</param>
				/// <returns></returns>
				HRESULT Create(D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA *resourceDesc, ID3D11Device *device);
			};
		}
	}
}

#endif

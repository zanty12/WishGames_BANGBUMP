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
* include宣言
------------------------------------------------------------------------------*/
#include <d3d11.h>
#include "dx_device_model.h"
#include "dx11_renderer.h"
#pragma comment(lib, "d3d11.lib")

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
		class Device3D : public DEVICE_MODEL<ID3D11Device, ID3D11DeviceContext> {
		public:

			/// <summary>
			/// デバイス3Dの作成
			/// </summary>
			/// <returns></returns>
			HRESULT Create() override;

			/// <summary>
			/// ターゲットする
			/// </summary>
			/// <param name="renderer">レンダラー</param>
			void Target(Renderer &renderer);

			/// <summary>
			/// バッファーの設定
			/// </summary>
			/// <param name="slot">スロット番号</param>
			/// <param name="buffNum">バッファ数</param>
			/// <param name="buffer">バッファ</param>
			static HRESULT SetConstantBuffer(UINT slot, UINT buffNum, ID3D11Buffer *buffer);

			/// <summary>
			/// バッファーの更新
			/// </summary>
			/// <param name="src">ソース</param>
			static HRESULT UpdateConstantBuffer(void *src, ID3D11Buffer* buffer);

			/// <summary>
			/// サンプラーの設定
			/// </summary>
			/// <param name="sampler">サンプラー</param>
			static HRESULT SetResource(ID3D11SamplerState *sampler);

			/// <summary>
			/// ラスタライザーの設定
			/// </summary>
			/// <param name="rasterizer">ラスタライザー</param>
			static HRESULT SetResource(ID3D11RasterizerState *rasterizer);

			/// <summary>
			/// ブレンドの設定
			/// </summary>
			/// <param name="blend">ブレンド</param>
			static HRESULT SetResource(ID3D11BlendState *blend);

			/// <summary>
			/// 深度ステンシルの設定
			/// </summary>
			/// <param name="depthstencil">深度ステンシル</param>
			static HRESULT SetResource(ID3D11DepthStencilState *depthstencil);

			/// <summary>
			/// リソースの設定
			/// </summary>
			/// <param name="resource">リソース</param>
			static HRESULT SetResource(ID3D11ShaderResourceView *resource);

			/// <summary>
			/// リソースの設定
			/// </summary>
			/// <param name="shader">頂点シェーダー</param>
			/// <param name="inputLayout">入力レイアウト</param>
			static HRESULT SetResource(ID3D11VertexShader *shader, ID3D11InputLayout *inputLayout);

			/// <summary>
			/// リソースの設定
			/// </summary>
			/// <param name="shader">ピクセルシェーダー</param>
			static HRESULT SetResource(ID3D11PixelShader *shader);

			/// <summary>
			/// バッファに書き込む
			/// </summary>
			/// <param name="vertex">頂点バッファ</param>
			/// <param name="vertexNum">頂点数</param>
			/// <param name="vertexSize">頂点バッファのサイズ</param>
			/// <param name="index">インデックスバッファ</param>
			/// <param name="indexNum">インデックス数</param>
			/// <param name="indexSize">インデックスバッファのサイズ</param>
			/// <param name="deviceContext">デバイスコンテキスト</param>
			static HRESULT Draw(ID3D11Buffer *vertex, UINT vertexNum, UINT vertexStructSize, ID3D11Buffer *index, UINT indexNum, UINT indexStructSize);

			/// <summary>
			/// トポロジーの設定
			/// </summary>
			/// <param name="topology">トポロジー</param>
			/// <param name="deviceContext"></param>
			static void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
		};
	}
}

#endif

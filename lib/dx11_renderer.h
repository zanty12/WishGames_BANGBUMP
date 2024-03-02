/*==============================================================================

	Title		: GRAPHICAL
	Description : グラフィカル
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_GRAPHICAL_H_
#define _DX11_GRAPHICAL_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include <d3d11.h>
// 描画
#include "dx11_swapchain.h"
#include "dx11_viewport.h"
#include "dx11_depthstencilview.h"
#include "dx11_rtv.h"
// ステート
#include "dx11_blend.h"
#include "dx11_depthstencil.h"
#include "dx11_rasterizer.h"
#include "dx11_sampler.h"
// シェーダー
#include "dx11_vertex_shader.h"
#include "dx11_pixel_shader.h"
#include "dx11_inputlayout.h"
// バッファ
#include "dx11_constant_buffer.h"
#include "dx_matrix.h"

#include "stdlib.h"

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
		class Renderer {
		private:
			CORE::VIEWPORT viewport;				// ビューポート
			
			CORE::SWAPCHAIN swapchain;				// スワップチェーン
			CORE::DEPTHSTENCILVIEW depthstencilview;// 深度ステンシル
			CORE::RTV rtv;							// レンダ―ターゲットビュー

			CORE::BLEND blend;						// ブレンド
			CORE::DEPTHSTENCIL depthstencil;		// デプスステンシル
			CORE::RASTERIZER rasterizer;			// ラスタライズ
			CORE::SAMPLER sampler;					// サンプラ―



		public:
			~Renderer() { Release(); }

			/// <summary>
			/// スワップチェーンとレンダーターゲットビューの作成
			/// </summary>
			/// <param name="hwnd">ハンドルウィンドウ</param>
			/// <param name="device3d">デバイス3D</param>
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
			/// 頂点シェーダをセットする
			/// </summary>
			/// <param name="shader">頂点シェーダ</param>
			/// <param name="inputLayout">インプットレイアウト</param>
			/// <param name="deviceContext">デバイスコンテキスト</param>
			/// <returns></returns>
			HRESULT SetShader(ID3D11VertexShader *shader, ID3D11InputLayout *inputLayout, ID3D11DeviceContext* deviceContext);

			/// <summary>
			/// ピクセルシェーダをセットする
			/// </summary>
			/// <param name="shader">ピクセルシェーダ</param>
			/// <param name="deviceContext">デバイスコンテキスト</param>
			/// <returns></returns>
			HRESULT SetShader(ID3D11PixelShader *shader, ID3D11DeviceContext* deviceContext);

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
			HRESULT Draw(ID3D11Buffer *vertex, UINT vertexNum, UINT vertexStructSize, ID3D11Buffer *index, UINT indexNum, UINT indexStructSize, ID3D11DeviceContext *deviceContext);
			
			/// <summary>
			/// 画面のクリア
			/// </summary>
			/// <param name="color">クリアにする色</param>
			/// <param name="deviceContext">デバイスコンテキスト</param>
			HRESULT Clear(Color color, ID3D11DeviceContext *deviceContext);

			/// <summary>
			/// プリミティブトポロジーの設定
			/// </summary>
			/// <param name="topology">プリミティブトポロジー</param>
			/// <param name="deviceContext">デバイスコンテキスト</param>
			HRESULT SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology, ID3D11DeviceContext *deviceContext);

			/// <summary>
			/// 描画する
			/// </summary>
			void Present(void);

			/// <summary>
			/// 解放
			/// </summary>
			/// <param name=""></param>
			void Release(void);
		};
	}
}

#endif

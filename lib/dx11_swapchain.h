/*==============================================================================

	Title		: SWAPCHAIN
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_SWAPCHAIN_H_
#define _DX11_SWAPCHAIN_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include <dxgi.h>
#include "dx_dxparts.h"

#pragma comment(lib, "dxgi.lib")

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
			class SWAPCHAIN : public DX::CORE::DX_PARTS<IDXGISwapChain, DXGI_SWAP_CHAIN_DESC, ID3D11Device> {
			private:
				DXGI_SWAP_CHAIN_DESC desc;

			public:

				/// <summary>
				/// デフォルトデスクの取得
				/// </summary>
				/// <param name="hwnd">ハンドルウィンドウ</param>
				/// <returns></returns>
				static DXGI_SWAP_CHAIN_DESC Desc(HWND hwnd);

				/// <summary>
				/// ブレンドステートの作成
				/// </summary>
				/// <param name="desc">デスク情報</param>
				/// <param name="device">デバイスポインタ</param>
				/// <returns></returns>
				HRESULT Create(DXGI_SWAP_CHAIN_DESC desc, ID3D11Device *device) override;

				/// <summary>
				/// デスクの取得
				/// </summary>
				/// <returns></returns>
				DXGI_SWAP_CHAIN_DESC GetDesc() { return desc; }
			};
		}
	}
}

#endif

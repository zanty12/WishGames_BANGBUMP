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
* include�錾
------------------------------------------------------------------------------*/
#include <dxgi.h>
#include "dx_dxparts.h"

#pragma comment(lib, "dxgi.lib")

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
			class SWAPCHAIN : public DX::CORE::DX_PARTS<IDXGISwapChain, DXGI_SWAP_CHAIN_DESC, ID3D11Device> {
			private:
				DXGI_SWAP_CHAIN_DESC desc;

			public:

				/// <summary>
				/// �f�t�H���g�f�X�N�̎擾
				/// </summary>
				/// <param name="hwnd">�n���h���E�B���h�E</param>
				/// <returns></returns>
				static DXGI_SWAP_CHAIN_DESC Desc(HWND hwnd);

				/// <summary>
				/// �u�����h�X�e�[�g�̍쐬
				/// </summary>
				/// <param name="desc">�f�X�N���</param>
				/// <param name="device">�f�o�C�X�|�C���^</param>
				/// <returns></returns>
				HRESULT Create(DXGI_SWAP_CHAIN_DESC desc, ID3D11Device *device) override;

				/// <summary>
				/// �f�X�N�̎擾
				/// </summary>
				/// <returns></returns>
				DXGI_SWAP_CHAIN_DESC GetDesc() { return desc; }
			};
		}
	}
}

#endif

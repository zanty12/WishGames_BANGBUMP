#include "dx11_texture.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			D3D11_TEXTURE2D_DESC TEXTURE::DescDepthStencil(DXGI_SWAP_CHAIN_DESC scDesc) {
				D3D11_TEXTURE2D_DESC desc;
				ZeroMemory(&desc, sizeof(desc));				// 初期化
				desc.Width = scDesc.BufferDesc.Width;			// バッファの幅
				desc.Height = scDesc.BufferDesc.Height;			// バッファの高さ
				desc.MipLevels = 1;								// ミップレベル
				desc.ArraySize = 1;								// 配列のサイズ
				desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// フォーマット
				desc.Usage = D3D11_USAGE_DEFAULT;				// バッファの使用方法（書き込み不可）
				desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// バッファの種類（デプスステンシル用に指定）
				desc.CPUAccessFlags = 0;						// CPUにアクセスするか否か（CPUアクセス不可能）
				desc.MiscFlags = 0;
				// MSAAの設定
				desc.SampleDesc.Count = 1;						// ピクセルの取得数
				desc.SampleDesc.Quality = 0;					// クオリティ
				return desc;
			}

			HRESULT TEXTURE::Create(D3D11_TEXTURE2D_DESC desc, ID3D11Device *device) {
				NULL_CHECK(device);
				CREATE_CHECK(resource);
				HRESULT hr;

				hr = device->CreateTexture2D(&desc, NULL, &resource);
				return hr;
			}
		}
	}
}

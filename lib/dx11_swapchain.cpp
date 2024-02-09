#include "dx11_swapchain.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			DXGI_SWAP_CHAIN_DESC SWAPCHAIN::Desc(HWND hwnd, BOOL windowed)
			{
				DXGI_SWAP_CHAIN_DESC desc;
				RECT rect;												// レクト
				int width;												// 幅
				int height;												// 高さ
				// Rectを取得して幅と高さを求める
				GetClientRect(hwnd, &rect);
				width = rect.right - rect.left;
				height = rect.bottom - rect.top;

				// SwapChain
				ZeroMemory(&desc, sizeof(desc));
				desc.BufferCount = 2;									// バッファ数
				desc.BufferDesc.Width = width;							// スワップチェーンの幅
				desc.BufferDesc.Height = height;						// スワップチェーンの高さ
				desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// フォーマット
				desc.BufferDesc.RefreshRate.Numerator = 60;				// 1フレームあたりの処理（Denominator / Numerator）
				desc.BufferDesc.RefreshRate.Denominator = 1;
				desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// 使用方法（バッファをレンダ―ターゲットとして使用）
				// MSAAの設定
				desc.SampleDesc.Count = 1;								// マルチサンプリングのピクセル単位の数
				desc.SampleDesc.Quality = 0;							// マルチサンプリングの品質（アンチエイリアス）
				// ウィンドウの設定 
				desc.Windowed = windowed;								// ウィンドウモード
				desc.OutputWindow = hwnd;								// 出力するハンドルウィンドウ

				return desc;
			}

			HRESULT SWAPCHAIN::Create(DXGI_SWAP_CHAIN_DESC desc, ID3D11Device *device) {
				NULL_CHECK(device);
				CREATE_CHECK(resource);

				HRESULT hr;
				IDXGIDevice1 *dxgi = NULL;
				IDXGIAdapter *adapter = NULL;
				IDXGIFactory *factory = NULL;
				this->desc = desc;

				// DXGI -> Adapter -> Factory と取得して
				// スワップチェーンを作成する
				hr = device->QueryInterface(IID_PPV_ARGS(&dxgi));
				ERROR_CHECK(hr, "Error create dxgi");
				hr = dxgi->GetAdapter(&adapter);
				ERROR_CHECK(hr, "Error create adapter");
				hr = adapter->GetParent(IID_PPV_ARGS(&factory));
				ERROR_CHECK(hr, "Error create factory.");
				hr = factory->CreateSwapChain(device, &desc, &resource);
				ERROR_CHECK(hr, "Error create swapchain.");

				// メモリの開放
				dxgi->Release();
				adapter->Release();
				factory->Release();

				return hr;
			}
		}
	}
}

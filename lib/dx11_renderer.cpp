#include "dx11_renderer.h"

namespace DX {
	namespace DX11 {
		HRESULT Renderer::Create(HWND hwnd, BOOL windowed, ID3D11Device *device) {
			using namespace CORE;
			NULL_CHECK(device);
			NULL_CHECK(hwnd);
			
			// ViewPortの設定
			viewport = VIEWPORT(hwnd);
			// SwapChainの作成
			swapchain.Create(SWAPCHAIN::Desc(hwnd, windowed), device);
			// RenderTargetViewの作成
			rtv.Create(RTV::Desc(swapchain.GetDesc()), swapchain, device);
			// DepthStencilViewの作成
			depthstencilview.Create(DEPTHSTENCILVIEW::Desc(), swapchain.GetDesc(), device);
			// Blendの作成
			blend.Create(BLEND::Desc(), device);
			// DepthStencilの作成
			depthstencil.Create(DEPTHSTENCIL::Desc(), device);
			// Rasterizerの作成
			rasterizer.Create(RASTERIZER::Desc(), device);
			// Samplerの作成
			sampler.Create(SAMPLER::Desc(), device);

			return 0;
		}

		HRESULT Renderer::SetShader(ID3D11VertexShader *shader, ID3D11InputLayout *inputLayout, ID3D11DeviceContext *deviceContext) {
			NULL_CHECK(shader);
			NULL_CHECK(inputLayout);
			NULL_CHECK(deviceContext);
			HRESULT hr;
			
			// デバイスコンテキストに登録をする
			deviceContext->IASetInputLayout(inputLayout);
			deviceContext->VSSetShader(shader, NULL, 0);

			return 0;
		}

		HRESULT Renderer::SetShader(ID3D11PixelShader *shader, ID3D11DeviceContext *deviceContext) {
			NULL_CHECK(shader);
			NULL_CHECK(deviceContext);
			HRESULT hr;

			// デバイスコンテキストに登録をする
			deviceContext->PSSetShader(shader, NULL, 0);

			return 0;
		}

		HRESULT Renderer::Draw(ID3D11Buffer *vertex, UINT vertexNum, UINT vertexStructSize, ID3D11Buffer *index, UINT indexNum, UINT indexStructSize, ID3D11DeviceContext *deviceContext) {
			// 頂点バッファ
			if (vertex) {
				UINT stride = vertexStructSize;
				UINT offset = 0;
				deviceContext->IASetVertexBuffers(0, 1, &vertex, &stride, &offset);
				deviceContext->Draw(vertexNum, 0);
			}
			// インデックスバッファ
			if (index) {
				UINT stride = indexStructSize;
				UINT offset = 0;
				deviceContext->IASetIndexBuffer(index, DXGI_FORMAT_R16_UINT, offset);
				deviceContext->DrawIndexed(indexNum, 0, 0);
			}

			return 0;
		}

		HRESULT Renderer::Clear(Color color, ID3D11DeviceContext *deviceContext) {
			NULL_CHECK(deviceContext);
			HRESULT hr;

			float vector4[4] = { color.r, color.g, color.b, color.a };
			deviceContext->ClearRenderTargetView(rtv, vector4);
			deviceContext->ClearDepthStencilView(depthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);
			return 0;
		}

		HRESULT Renderer::SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology, ID3D11DeviceContext *deviceContext) {
			NULL_CHECK(deviceContext);
			deviceContext->IASetPrimitiveTopology(topology);
			return 0;
		}

		void Renderer::Present(void) {
			// スワップチェーンを切り替えて描画する
			swapchain->Present(1, 0);
		}

		void Renderer::Release(void) {
			// Blendの解放
			blend.Release();
			// DepthStencilの解放
			depthstencil.Release();
			// Rasterizerの解放
			rasterizer.Release();
			// Samplerの解放
			sampler.Release();
			// DepthStencilViewの解放
			depthstencilview.Release();
			// RenderTargetViewの解放
			rtv.Release();
			// SwapChainの解放
			swapchain.Release();
		}
	};
};
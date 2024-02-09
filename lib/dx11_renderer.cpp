#include "dx11_renderer.h"

namespace DX {
	namespace DX11 {
		HRESULT Renderer::Create(HWND hwnd, BOOL windowed, ID3D11Device *device) {
			using namespace CORE;
			NULL_CHECK(device);
			NULL_CHECK(hwnd);
			
			// ViewPort�̐ݒ�
			viewport = VIEWPORT(hwnd);
			// SwapChain�̍쐬
			swapchain.Create(SWAPCHAIN::Desc(hwnd, windowed), device);
			// RenderTargetView�̍쐬
			rtv.Create(RTV::Desc(swapchain.GetDesc()), swapchain, device);
			// DepthStencilView�̍쐬
			depthstencilview.Create(DEPTHSTENCILVIEW::Desc(), swapchain.GetDesc(), device);
			// Blend�̍쐬
			blend.Create(BLEND::Desc(), device);
			// DepthStencil�̍쐬
			depthstencil.Create(DEPTHSTENCIL::Desc(), device);
			// Rasterizer�̍쐬
			rasterizer.Create(RASTERIZER::Desc(), device);
			// Sampler�̍쐬
			sampler.Create(SAMPLER::Desc(), device);

			return 0;
		}

		HRESULT Renderer::SetShader(ID3D11VertexShader *shader, ID3D11InputLayout *inputLayout, ID3D11DeviceContext *deviceContext) {
			NULL_CHECK(shader);
			NULL_CHECK(inputLayout);
			NULL_CHECK(deviceContext);
			HRESULT hr;
			
			// �f�o�C�X�R���e�L�X�g�ɓo�^������
			deviceContext->IASetInputLayout(inputLayout);
			deviceContext->VSSetShader(shader, NULL, 0);

			return 0;
		}

		HRESULT Renderer::SetShader(ID3D11PixelShader *shader, ID3D11DeviceContext *deviceContext) {
			NULL_CHECK(shader);
			NULL_CHECK(deviceContext);
			HRESULT hr;

			// �f�o�C�X�R���e�L�X�g�ɓo�^������
			deviceContext->PSSetShader(shader, NULL, 0);

			return 0;
		}

		HRESULT Renderer::Draw(ID3D11Buffer *vertex, UINT vertexNum, UINT vertexStructSize, ID3D11Buffer *index, UINT indexNum, UINT indexStructSize, ID3D11DeviceContext *deviceContext) {
			// ���_�o�b�t�@
			if (vertex) {
				UINT stride = vertexStructSize;
				UINT offset = 0;
				deviceContext->IASetVertexBuffers(0, 1, &vertex, &stride, &offset);
				deviceContext->Draw(vertexNum, 0);
			}
			// �C���f�b�N�X�o�b�t�@
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
			// �X���b�v�`�F�[����؂�ւ��ĕ`�悷��
			swapchain->Present(1, 0);
		}

		void Renderer::Release(void) {
			// Blend�̉��
			blend.Release();
			// DepthStencil�̉��
			depthstencil.Release();
			// Rasterizer�̉��
			rasterizer.Release();
			// Sampler�̉��
			sampler.Release();
			// DepthStencilView�̉��
			depthstencilview.Release();
			// RenderTargetView�̉��
			rtv.Release();
			// SwapChain�̉��
			swapchain.Release();
		}
	};
};
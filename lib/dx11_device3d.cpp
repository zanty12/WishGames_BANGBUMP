#include "dx11_device3d.h"

ID3D11Device *DX::DX11::Device3D::device = nullptr;
ID3D11DeviceContext *DX::DX11::Device3D::context = nullptr;

namespace DX {
	namespace DX11 {
		HRESULT Device3D::Create() {
			CREATE_CHECK(device);
			CREATE_CHECK(context);

			HRESULT hr = S_OK;									// OK���
			UINT flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;		// Device2D�T�|�[�g
			D3D_FEATURE_LEVEL featureLevel;						// ���̃A�v���P�[�V�����̑Ή����Ă���o�[�W����
			D3D_FEATURE_LEVEL featureLevels[] = {				// �A�v���P�[�V�����Ή��o�[�W����
				D3D_FEATURE_LEVEL_11_1,							// 11.1  ShaderModel 5
				D3D_FEATURE_LEVEL_11_0,							// 11.0  ShaderModel 5
				D3D_FEATURE_LEVEL_10_1,							// 10.1  ShaderModel 4
				D3D_FEATURE_LEVEL_10_0,							// 10.0  ShaderModel 4
				D3D_FEATURE_LEVEL_9_3,							// 9.3   ShaderModel 3
				D3D_FEATURE_LEVEL_9_2,							// 9.2   ShaderModel 3
				D3D_FEATURE_LEVEL_9_1,							// 9.1   ShaderModel 3
			};
			int featureLevelsNum = ARRAYSIZE(featureLevels);	// �Ή��o�[�W�����̗v�f��
#ifdef _DEBUG
			flag |= D3D11_CREATE_DEVICE_DEBUG;					// �f�o�b�O
#endif

			// 3D�f�o�C�X���쐬
			hr = D3D11CreateDevice(
				NULL,											// �A�_�v�^�i�g�p���Ȃ��ꍇ��NULL�j
				D3D_DRIVER_TYPE_HARDWARE,						// �h���C�o�^�C�v
				NULL,											// �\�t�g�E�F�A�i�f�t�H���g��NULL�j
				flag,											// �t���O
				featureLevels,									// ���̃A�v���P�[�V�����őΉ����Ă���o�[�W����
				featureLevelsNum,								// featureLevels�̗v�f��
				D3D11_SDK_VERSION,								// SDK�o�[�W����
				&device,										// �쐬���ꂽ�f�o�C�X��Ԃ�
				&featureLevel,									// ����GPU���Ή����Ă���DirectX�̃o�[�W�����̍ő��Ԃ�
				&context										// �쐬���ꂽ�f�o�C�X�R���e�L�X�g��Ԃ�
			);

			return hr;
		}

		void Device3D::Target(Renderer &renderer) {
			// �^�[�Q�b�g�ɂ���
			ID3D11RenderTargetView *rtv = renderer.GetRTV();
			context->OMSetRenderTargets(1, &rtv, NULL);

			// �r���[�|�[�g�̐ݒ�
			D3D11_VIEWPORT vp = renderer.GetViewport();
			context->RSSetViewports(1, &vp);
		}

		HRESULT Device3D::SetConstantBuffer(UINT slot, UINT buffNum, ID3D11Buffer *buffer) {
			NULL_CHECK(buffer);

			context->VSSetConstantBuffers(slot, buffNum, &buffer);
			return 0;
		}

		HRESULT Device3D::UpdateConstantBuffer(void *src, ID3D11Buffer *buffer) {
			NULL_CHECK(src);
			NULL_CHECK(buffer);

			context->UpdateSubresource(buffer, 0, nullptr, src, 0, 0);
			return 0;
		}

		HRESULT Device3D::SetResource(ID3D11SamplerState *sampler) {
			NULL_CHECK(sampler);

			context->PSSetSamplers(0, 1, &sampler);
			return 0;
		}

		HRESULT Device3D::SetResource(ID3D11RasterizerState *rasterizer) {
			NULL_CHECK(rasterizer);

			context->RSSetState(rasterizer);
			return 0;
		}

		HRESULT Device3D::SetResource(ID3D11BlendState *blend) {
			NULL_CHECK(blend);
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

			context->OMSetBlendState(blend, blendFactor, 0xffffffff);
			return 0;
		}

		HRESULT Device3D::SetResource(ID3D11DepthStencilState *depthstencil) {
			NULL_CHECK(depthstencil);

			context->OMSetDepthStencilState(depthstencil, NULL);
			return 0;
		}

		HRESULT Device3D::SetResource(ID3D11ShaderResourceView *resource) {
			NULL_CHECK(resource);
			
			context->PSSetShaderResources(0, 1, &resource);
		}


		HRESULT Device3D::SetResource(ID3D11VertexShader *shader, ID3D11InputLayout *inputLayout) {
			NULL_CHECK(shader);
			NULL_CHECK(inputLayout);
			HRESULT hr;

			// �f�o�C�X�R���e�L�X�g�ɓo�^������
			context->IASetInputLayout(inputLayout);
			context->VSSetShader(shader, NULL, 0);

			return 0;
		}

		HRESULT Device3D::SetResource(ID3D11PixelShader *shader) {
			NULL_CHECK(shader);
			HRESULT hr;

			// �f�o�C�X�R���e�L�X�g�ɓo�^������
			context->PSSetShader(shader, NULL, 0);

			return 0;
		}

		HRESULT Device3D::Draw(ID3D11Buffer *vertex, UINT vertexNum, UINT vertexStructSize, ID3D11Buffer *index, UINT indexNum, UINT indexStructSize) {
			// ���_�o�b�t�@
			if (vertex) {
				UINT stride = vertexStructSize;
				UINT offset = 0;
				context->IASetVertexBuffers(0, 1, &vertex, &stride, &offset);
				context->Draw(vertexNum, 0);
			}
			// �C���f�b�N�X�o�b�t�@
			if (index) {
				UINT stride = indexStructSize;
				UINT offset = 0;
				context->IASetIndexBuffer(index, DXGI_FORMAT_R16_UINT, offset);
				context->DrawIndexed(indexNum, 0, 0);
			}

			return 0;
		}

		void Device3D::SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology) {
			context->IASetPrimitiveTopology(topology);
		}
	};
};

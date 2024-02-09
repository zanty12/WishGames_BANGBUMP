#include "dx11_sampler.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			D3D11_SAMPLER_DESC SAMPLER::Desc() {
				// https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_sampler_desc
				D3D11_SAMPLER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Filter = D3D11_FILTER_ANISOTROPIC;
				desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.MipLODBias = 0;
				desc.MaxAnisotropy = 16;
				desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
				desc.MinLOD = 0;
				desc.MaxLOD = D3D11_FLOAT32_MAX;

				return desc;
			}

			D3D11_SAMPLER_DESC SAMPLER::DescLinear() {
				D3D11_SAMPLER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.MipLODBias = 0;
				desc.MaxAnisotropy = 1;
				desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
				desc.MinLOD = 0;
				desc.MaxLOD = D3D11_FLOAT32_MAX;

				return desc;
			}

			HRESULT SAMPLER::Create(D3D11_SAMPLER_DESC desc, ID3D11Device *device) {
				NULL_CHECK(device);
				CREATE_CHECK(resource);
				HRESULT hr;

				hr = device->CreateSamplerState(&desc, &resource);
				return hr;
			}
		}
	}
};

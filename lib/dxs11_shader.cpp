#include "dxs11_shader.h"

namespace DX {
	namespace DX11 {
		namespace GRAPHICAL {
			HRESULT Shader::Create(SHADER_DESC desc) {
				using namespace DX::DX11::CORE;
				HRESULT hr;

				vertexShader.Compile(desc.vertexPath, desc.vertexEntryPoint, v5_0);
				vertexShader.Create(Device3D::Get());
				pixelShader.Compile(desc.pixelPath, desc.pixelEntryPoint, v5_0);
				pixelShader.Create(Device3D::Get());
				inputLayout.Create(desc.desc, desc.descNum, vertexShader, Device3D::Get());
				Device3D::SetResource(vertexShader, inputLayout);
				Device3D::SetResource(pixelShader);
				Update();

				return 0;
			}

			HRESULT Shader::Update(void) {
				Device3D::SetResource(vertexShader, inputLayout);
				Device3D::SetResource(pixelShader);

				return 0;
			}

			void Shader::Release(void) {
				vertexShader.Release();
				pixelShader.Release();
				inputLayout.Release();
			}
		}
	}
}
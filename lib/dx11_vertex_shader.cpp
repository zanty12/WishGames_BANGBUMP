#include "dx11_vertex_shader.h"
#include <string>

namespace DX {
    namespace DX11 {
        namespace CORE {
            HRESULT VERTEX_SHADER::Create(ID3D11Device *device) {
                NULL_CHECK(device);
                NULL_CHECK(blob);
                CREATE_CHECK(shader);
                HRESULT hr;

                hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);

                return hr;
            }

            HRESULT VERTEX_SHADER::Compile(LPCWSTR path, const char *entry, SHADER_VERSION version) {
                std::string ver;
                switch (version)
                {
                case v1_0: ver = "vs_1_0"; break;
                case v2_0: ver = "vs_2_0"; break;
                case v3_0: ver = "vs_3_0"; break;
                case v4_0: ver = "vs_4_0"; break;
                case v5_0: ver = "vs_5_0"; break;
                }

                return Shader::Compile(path, (char *)entry, (char *)ver.c_str());
            }
        }
    }
}

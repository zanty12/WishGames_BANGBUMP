#include "dx11_pixel_shader.h"
#include <string>

namespace DX {
    namespace DX11 {
        namespace CORE {
            HRESULT PIXEL_SHADER::Create(ID3D11Device *device) {
                NULL_CHECK(device);
                NULL_CHECK(blob);
                CREATE_CHECK(shader);
                HRESULT hr;

                hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);

                return hr;
            }

            HRESULT PIXEL_SHADER::Compile(LPCWSTR path, const char *entry, SHADER_VERSION version) {
                std::string ver;
                switch (version)
                {
                case v1_0: ver = "ps_1_0"; break;
                case v2_0: ver = "ps_2_0"; break;
                case v3_0: ver = "ps_3_0"; break;
                case v4_0: ver = "ps_4_0"; break;
                case v5_0: ver = "ps_5_0"; break;
                }

                return Shader::Compile(path, (char *)entry, (char *)ver.c_str());
            }
        }
    }
}

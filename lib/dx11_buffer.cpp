#include "dx11_buffer.h"

namespace DX {
    namespace DX11 {
        namespace CORE {
            D3D11_BUFFER_DESC Buffer::Desc(UINT structSize, D3D11_USAGE usage, D3D11_BIND_FLAG bindFlag, UINT cpuAccessFlag, UINT stride) {
                // https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_buffer_desc
                D3D11_BUFFER_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.ByteWidth = (UINT)structSize;
                desc.Usage = usage;
                desc.BindFlags = bindFlag;
                desc.CPUAccessFlags = cpuAccessFlag;
                desc.MiscFlags = 0;
                desc.StructureByteStride = (UINT)stride;

                return desc;
            }

            D3D11_BUFFER_DESC Buffer::ConstantDesc(UINT structSize, UINT stride) {
                D3D11_BUFFER_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.ByteWidth = structSize;
                desc.Usage = D3D11_USAGE_DEFAULT;
                desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = 0;
                desc.MiscFlags = 0;
                desc.StructureByteStride = stride;

                return desc;
            }

            D3D11_BUFFER_DESC Buffer::VertexDesc(UINT vertexSize) {
                D3D11_BUFFER_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.ByteWidth = (UINT)vertexSize;
                desc.Usage = D3D11_USAGE_DYNAMIC;
                desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                desc.MiscFlags = 0;
                desc.StructureByteStride = 0;

                return desc;
            }

            D3D11_BUFFER_DESC Buffer::IndexDesc(UINT indexSize) {
                D3D11_BUFFER_DESC desc;
                ZeroMemory(&desc, sizeof(desc));
                desc.ByteWidth = (UINT)indexSize;
                desc.Usage = D3D11_USAGE_DYNAMIC;
                desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                desc.MiscFlags = 0;
                desc.StructureByteStride = 0;

                return desc;
            }

            HRESULT Buffer::Create(D3D11_BUFFER_DESC desc, ID3D11Device *device) {
                NULL_CHECK(device);
                CREATE_CHECK(resource);
                HRESULT hr;

                hr = device->CreateBuffer(&desc, NULL, &this->resource);
                return hr;
            }

            HRESULT Buffer::Create(D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA* resourceDesc, ID3D11Device *device) {
                NULL_CHECK(device);
                CREATE_CHECK(resource);
                HRESULT hr;

                hr = device->CreateBuffer(&desc, resourceDesc, &resource);
                return hr;
            }
        }
    }
};

#include "dx11_inputlayout.h"

namespace DX {
	namespace DX11 {
		namespace CORE {
			D3D11_INPUT_ELEMENT_DESC INPUTLAYOUT::Desc(const char *name, UINT slot, INPUT_LAYOUT_FORMAT format) {
				// https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_input_element_desc
				D3D11_INPUT_ELEMENT_DESC desc;

				desc.SemanticName = name;															// 名前
				desc.SemanticIndex = 0;																// セマンティックインデックス
				switch (format) {																	// フォーマット
				case FORMAT_FLOAT1: desc.Format = DXGI_FORMAT_R32_FLOAT; break;
				case FORMAT_FLOAT2: desc.Format = DXGI_FORMAT_R32G32_FLOAT; break;
				case FORMAT_FLOAT3: desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
				case FORMAT_FLOAT4: desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
				}
				desc.InputSlot = slot;																// 頂点バッファのスロット
				desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;								// オフセット
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;									// スロットの入力クラス
				desc.InstanceDataStepRate = 0;														// 頂点データの再利用数

				return desc;
			}

			D3D11_INPUT_ELEMENT_DESC INPUTLAYOUT::Desc(const char *name, UINT slot, UINT startBytePosition, INPUT_LAYOUT_FORMAT format) {
				// https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_input_element_desc
				D3D11_INPUT_ELEMENT_DESC desc;

				desc.SemanticName = name;															// 名前
				desc.SemanticIndex = 0;																// セマンティックインデックス
				switch (format) {																	// フォーマット
				case FORMAT_FLOAT1: desc.Format = DXGI_FORMAT_R32_FLOAT; break;
				case FORMAT_FLOAT2: desc.Format = DXGI_FORMAT_R32G32_FLOAT; break;
				case FORMAT_FLOAT3: desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
				case FORMAT_FLOAT4: desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
				}
				desc.InputSlot = slot;																// 頂点バッファのスロット
				desc.AlignedByteOffset = startBytePosition;											// オフセット
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;									// スロットの入力クラス
				desc.InstanceDataStepRate = 0;														// 頂点データの再利用数

				return desc;
			}

			HRESULT INPUTLAYOUT::Create(D3D11_INPUT_ELEMENT_DESC desc[], UINT descNum, ID3DBlob *blob, ID3D11Device *device) {
				NULL_CHECK(desc);
				NULL_CHECK(blob);
				NULL_CHECK(device);
				CREATE_CHECK(resource);
				HRESULT hr;

				hr = device->CreateInputLayout(
					desc,																			// データ構造の情報
					descNum,																		// descの要素数
					blob->GetBufferPointer(),														// 頂点シェーダのコンパイルされたデータのポインタ
					blob->GetBufferSize(),															// 頂点シェーダのコンパイルされたデータのサイズ
					&resource
				);

				return hr;
			}
		}
	}
}

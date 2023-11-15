/*==============================================================================

	Title		: VERTEX_BUFFER
	Description : 頂点構造体
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_VERTEX_BUFFER_H_
#define _DX11_VERTEX_BUFFER_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include "dx11_primitive_buffer.h"
#include "dx11_vertex_struct.h"

/*------------------------------------------------------------------------------
* define宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class宣言
------------------------------------------------------------------------------*/
namespace DX {
	namespace DX11 {
		namespace CORE {
			/// <summary>
			/// 頂点バッファ
			/// </summary>
			/// <typeparam name="T"></typeparam>
			template <class T>
			class VertexBuffer : public PrimitiveBuffer<T> {
			public:

				/// <summary>
				/// 頂点バッファの作成
				/// </summary>
				/// <param name="vertex">頂点配列</param>
				/// <param name="vertexNum">頂点配列の要素数</param>
				/// <param name="device">デバイスポインター</param>
				/// <returns></returns>
				HRESULT Create(T *vertex, UINT vertexNum, PRIMITIVE_SET_TYPE setType, ID3D11Device *device) {
					NULL_CHECK(device);
					HRESULT hr;

					// サブリソース
					D3D11_SUBRESOURCE_DATA desc;
					desc.pSysMem = vertex;
					desc.SysMemPitch = sizeof(T);
					desc.SysMemSlicePitch = 0;

					// バッファの作成
					UINT vertexSize = sizeof(T) * vertexNum;
					hr = Buffer::Create(Buffer::VertexDesc(vertexSize), &desc, device);
					ERROR_CHECK(hr, "Error create buffer.");

					// リソースの作成
					hr = PrimitiveBuffer<T>::CreateResource(vertex, vertexNum, setType);

					return hr;
				}
			};
		}
	}
}

#endif

/*==============================================================================

	Title		: PRIMITIVE
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_PRIMITIVE_H_
#define _DX11_PRIMITIVE_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include <string.h>
#include "dx11_vertex_buffer.h"
#include "dx11_index_buffer.h"

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
		namespace GRAPHICAL {
			enum BUFFER_TYPE {
				BUFFER_TYPE_NONE,
				BUFFER_TYPE_VERTEX_ONLY,
				BUFFER_TYPE_VERTEX_AND_INDEX
			};

			template<class T>
			class Primitive {
			private:
				CORE::VertexBuffer<T> vertexBuffer;			// 頂点バッファ
				CORE::IndexBuffer indexBuffer;					// インデックスバッファ

			public:

				~Primitive() { Release(); }

				/// <summary>
				/// バッファの作成
				/// </summary>
				/// <param name="vertex">頂点バッファ配列</param>
				/// <param name="vertexNum">頂点バッファ数</param>
				/// <param name="index">インデックスバッファ配列</param>
				/// <param name="indexNum">インデックスバッファ数</param>
				/// <param name="setType">タイプ</param>
				/// <returns></returns>
				HRESULT Create(T *vertex, UINT vertexNum, DWORD *index, UINT indexNum, PRIMITIVE_SET_TYPE setType) {
					NULL_CHECK(vertex);
					CREATE_CHECK((ID3D11Buffer *)vertexBuffer);
					CREATE_CHECK((ID3D11Buffer *)indexBuffer);

					// バッファの作成
					if (vertex) vertexBuffer.Create(vertex, vertexNum, setType, Device3D::Get());
					if (index) indexBuffer.Create(index, indexNum, setType, Device3D::Get());
					
					return 0;
				}

				/// <summary>
				/// 解放
				/// </summary>
				void Release(void) {
					vertexBuffer.Release();
					indexBuffer.Release();
				}

				/// <summary>
				/// 更新
				/// </summary>
				void Update(void) {
					ID3D11Buffer *vertexBuffer = GetVertexBuffer();
					ID3D11Buffer *indexBuffer = GetIndexBuffer();
					// 頂点
					if (vertexBuffer) {
						UINT stride = sizeof(T);																// 1頂点の領域
						UINT offset = 0;																			// 1頂点の使用しない領域（ダミー）
						// 頂点バッファの入力
						Device3D::GetContext()->IASetVertexBuffers(
							0,
							1,
							&vertexBuffer,																			// 頂点バッファ
							&stride,																				// (stride + offset)のサイズ分ポインタをずらす
							&offset
						);

						// 頂点リソース
						D3D11_MAPPED_SUBRESOURCE vertexResource;
						Device3D::GetContext()->Map(
							vertexBuffer,																			// 頂点バッファ
							0,																						// サブリソース
							D3D11_MAP_WRITE_DISCARD,																// マップタイプ（バッファに書き込むためにGPUのアクセスを拒否する）
							0,																						// フラグ
							&vertexResource																			// リソース
						);

						// 頂点データの取得
						T *fromVertex = GetVertexPointer();
						T *toVertex = (T *)vertexResource.pData;
						// データのコピー
						memcpy(toVertex, fromVertex, sizeof(T) * GetVertexCount());


						// GPUにアクセスを許可する
						Device3D::GetContext()->Unmap(vertexBuffer, 0);
					}
					// インデックス
					if (indexBuffer) {
						UINT offset = 0;																			// 1インデックスの使用しない領域（ダミー）
						ID3D11Buffer *indexBuffer = GetIndexBuffer();

						// 頂点バッファの入力
						Device3D::GetContext()->IASetIndexBuffer(
							indexBuffer,																			// インデックスバッファ
							DXGI_FORMAT_R16_UINT,																	// インデックスバッファのフォーマット
							offset																					// offsetのダミー分ずらす
						);

						// インデックスリソース
						D3D11_MAPPED_SUBRESOURCE indexResource;
						Device3D::GetContext()->Map(
							indexBuffer,																			// インデックスバッファ
							0,																						// サブリソース
							D3D11_MAP_WRITE_DISCARD,																// マップタイプ（バッファに書き込むためにGPUのアクセスを拒否する）
							0,																						// フラグ
							&indexResource																			// リソース
						);

						// インデックスデータの取得
						DWORD *fromIndex = (DWORD *)GetIndexPointer();
						DWORD *toIndex = (DWORD *)indexResource.pData;
						// データのコピー
						memcpy(toIndex, fromIndex, sizeof(DWORD) * GetIndexCount());

						// GPUにアクセスを許可する
						Device3D::GetContext()->Unmap(indexBuffer, 0);
					}
				}

				/// <summary>
				/// 頂点数の取得
				/// </summary>
				/// <returns></returns>
				UINT GetVertexCount(void) { return vertexBuffer.GetCount(); }

				/// <summary>
				/// インデックス数の取得
				/// </summary>
				/// <returns></returns>
				UINT GetIndexCount(void) { return indexBuffer.GetCount(); }

				/// <summary>
				/// 頂点構造体のサイズの取得
				/// </summary>
				/// <returns></returns>
				UINT GetVertexStructByteSize(void) { return sizeof(T); }

				/// <summary>
				/// インデックス構造体のサイズの取得
				/// </summary>
				/// <returns></returns>
				UINT GetIndexStructByteSize(void) { return sizeof(DWORD); }

				/// <summary>
				/// 頂点バッファの取得
				/// </summary>
				/// <returns></returns>
				ID3D11Buffer *GetVertexBuffer(void) { return vertexBuffer; }

				/// <summary>
				/// インデックスバッファの取得
				/// </summary>
				/// <returns></returns>
				ID3D11Buffer *GetIndexBuffer(void) { return indexBuffer; }

				/// <summary>
				/// 頂点ポインタの取得
				/// </summary>
				/// <returns></returns>
				T *GetVertexPointer(void) { return vertexBuffer.GetResource(0); }

				/// <summary>
				/// インデックスポインタの取得
				/// </summary>
				/// <returns></returns>
				DWORD *GetIndexPointer(void) { return indexBuffer.GetResource(0); }
			};
		}
	}
}

#endif

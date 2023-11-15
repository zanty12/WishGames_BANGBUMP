/*==============================================================================

	Title		: PRIMITIVE_BUFFER
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_PRIMITIVE_BUFFER_H_
#define _DX11_PRIMITIVE_BUFFER_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include "dx11_buffer.h"

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

		enum PRIMITIVE_SET_TYPE {
			PRIMITIVE_SET_TYPE_NONE,
			PRIMITIVE_SET_TYPE_COPY,
			PRIMITIVE_SET_TYPE_POINTER,
		};

		namespace CORE {
			/// <summary>
			/// プリミティブに関連するバッファ
			/// </summary>
			/// <typeparam name="T"></typeparam>
			template<class T>
			class PrimitiveBuffer : public Buffer {
			protected:
				T *pResource = nullptr;									// バッファ
				UINT resourceNum = 0;									// バッファの要素数
				PRIMITIVE_SET_TYPE setType = PRIMITIVE_SET_TYPE_NONE;	// セットタイプ

			public:

				/// <summary>
				/// リソースの作成
				/// </summary>
				/// <param name="pResource">リソース</param>
				/// <param name="resourceNum">要素数</param>
				/// <param name="setType">タイプ</param>
				HRESULT CreateResource(T *pResource, UINT resourceNum, PRIMITIVE_SET_TYPE setType) {
					NULL_CHECK(pResource);
					CREATE_CHECK(this->pResource);

					if (setType == PRIMITIVE_SET_TYPE_NONE) return -1;
					this->resourceNum = resourceNum;
					this->setType = setType;

					// リソースの作成
					switch (setType) {
					case PRIMITIVE_SET_TYPE_COPY:
						this->pResource = new T[resourceNum];
						memcpy(this->pResource, pResource, sizeof(T) * resourceNum);
						break;

					case PRIMITIVE_SET_TYPE_POINTER:
						this->pResource = pResource;
						break;
					}
					
					return 0;
				}

				/// <summary>
				/// リソースの解放
				/// </summary>
				void Release(void) override {
					// 解放
					Buffer::Release();
					if(setType == PRIMITIVE_SET_TYPE_COPY) 
						if (pResource) delete pResource;

					pResource = nullptr;
					resourceNum = 0;
					setType = PRIMITIVE_SET_TYPE_NONE;
				}

				/// <summary>
				/// 長さの取得
				/// </summary>
				unsigned int GetCount(void) { return resourceNum; }

				/// <summary>
				/// 取得
				/// </summary>
				/// <param name="index">インデックス</param>
				T *GetResource(UINT index) { if (index < resourceNum) return &pResource[index]; return nullptr; }

				/// <summary>
				/// セット
				/// </summary>
				/// <param name="index">インデックス</param>
				/// <param name="resource">リソース</param>
				void SetResource(UINT index, T resource) { if (index < resourceNum) pResource[index] = resource; }
			};
		}
	}
}

#endif

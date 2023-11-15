/*==============================================================================

	Title		: DX_PARTS
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX_DX_PARTS_H_
#define _DX_DX_PARTS_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include <d3d11.h>
#include "errormsg.h"


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
	namespace CORE {
		template<class T, class DESC, class DEVICE>
		class DX_PARTS {
		public:
			T *resource = NULL;						// リソース



			DX_PARTS() { }
			~DX_PARTS() { Release(); }

			/// <summary>
			/// 作成
			/// </summary>
			/// <param name="desc">デスク情報</param>
			/// <param name="Device">デバイスポインタ</param>
			/// <returns></returns>
			virtual HRESULT Create(DESC desc, DEVICE *device) { return 0; };

			/// <summary>
			/// 取得
			/// </summary>
			/// <returns></returns>
			T *Get() { return resource; }

			operator T *() { return resource; }
			T *operator ->() { return resource; }

			/// <summary>
			/// 解放
			/// </summary>
			virtual void Release() {
				if (resource) resource->Release();
				resource = nullptr;
			}
		};

		template<class T, class DEVICE>
		class DX_PARTS<T, void, DEVICE> {
		public:
			T *resource = NULL;						// リソース



			DX_PARTS() { }
			~DX_PARTS() { Release(); }

			/// <summary>
			/// 作成
			/// </summary>
			/// <param name="Device">デバイスポインタ</param>
			/// <returns></returns>
			virtual HRESULT Create(DEVICE *device) { return 0; };

			/// <summary>
			/// 取得
			/// </summary>
			/// <returns></returns>
			T *Get() { return resource; }

			operator T *() { return resource; }
			T *operator ->() { return resource; }

			/// <summary>
			/// 解放
			/// </summary>
			virtual void Release() {
				if (resource) resource->Release();
				resource = nullptr;
			}
		};
	}
}

#endif

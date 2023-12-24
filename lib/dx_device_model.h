/*==============================================================================

	Title		: DEVICE_MODEL
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DEVICE_MODEL_H_
#define _DEVICE_MODEL_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include <d3d.h>

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
	template<class D, class DC>
	class DEVICE_MODEL {
	protected:
		static D *device;
		static DC *context;

	public:

		virtual ~DEVICE_MODEL() { Release(); }

		/// <summary>
		/// デバイスの作成
		/// </summary>
		/// <returns></returns>
		virtual HRESULT Create() { return 0; };

		/// <summary>
		/// デバイスの取得
		/// </summary>
		/// <returns></returns>
		static D *Get() { return device; }

		/// <summary>
		/// デバイスコンテキストの取得
		/// </summary>
		/// <returns></returns>
		static DC *GetContext() { return context; }

		/// <summary>
		/// 解放
		/// </summary>
		void Release() {
			if (device) device->Release();
			if (context) context->Release();
			device = nullptr;
			context = nullptr;
		}

		operator D *() { return device; }
		operator DC *() { return context; }
	};
}

#endif

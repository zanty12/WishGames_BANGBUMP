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
* include�錾
------------------------------------------------------------------------------*/
#include <d3d.h>

/*------------------------------------------------------------------------------
* define�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* extern�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototype�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class�錾
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
		/// �f�o�C�X�̍쐬
		/// </summary>
		/// <returns></returns>
		virtual HRESULT Create() { return 0; };

		/// <summary>
		/// �f�o�C�X�̎擾
		/// </summary>
		/// <returns></returns>
		static D *Get() { return device; }

		/// <summary>
		/// �f�o�C�X�R���e�L�X�g�̎擾
		/// </summary>
		/// <returns></returns>
		static DC *GetContext() { return context; }

		/// <summary>
		/// ���
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

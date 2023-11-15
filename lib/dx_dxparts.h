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
* include�錾
------------------------------------------------------------------------------*/
#include <d3d11.h>
#include "errormsg.h"


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
	namespace CORE {
		template<class T, class DESC, class DEVICE>
		class DX_PARTS {
		public:
			T *resource = NULL;						// ���\�[�X



			DX_PARTS() { }
			~DX_PARTS() { Release(); }

			/// <summary>
			/// �쐬
			/// </summary>
			/// <param name="desc">�f�X�N���</param>
			/// <param name="Device">�f�o�C�X�|�C���^</param>
			/// <returns></returns>
			virtual HRESULT Create(DESC desc, DEVICE *device) { return 0; };

			/// <summary>
			/// �擾
			/// </summary>
			/// <returns></returns>
			T *Get() { return resource; }

			operator T *() { return resource; }
			T *operator ->() { return resource; }

			/// <summary>
			/// ���
			/// </summary>
			virtual void Release() {
				if (resource) resource->Release();
				resource = nullptr;
			}
		};

		template<class T, class DEVICE>
		class DX_PARTS<T, void, DEVICE> {
		public:
			T *resource = NULL;						// ���\�[�X



			DX_PARTS() { }
			~DX_PARTS() { Release(); }

			/// <summary>
			/// �쐬
			/// </summary>
			/// <param name="Device">�f�o�C�X�|�C���^</param>
			/// <returns></returns>
			virtual HRESULT Create(DEVICE *device) { return 0; };

			/// <summary>
			/// �擾
			/// </summary>
			/// <returns></returns>
			T *Get() { return resource; }

			operator T *() { return resource; }
			T *operator ->() { return resource; }

			/// <summary>
			/// ���
			/// </summary>
			virtual void Release() {
				if (resource) resource->Release();
				resource = nullptr;
			}
		};
	}
}

#endif

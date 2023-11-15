/*==============================================================================

	Title		: GPU_ACCESS
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11S_GPU_ACCESS_H_
#define _DX11S_GPU_ACCESS_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include "dx11_constant_buffer.h"
#include "dx11_device3d.h"
#include "dx_matrix.h"

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
* enum�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class�錾
------------------------------------------------------------------------------*/
namespace DX {
	namespace DX11 {
		template<class T>
		class GPU_ACCESS {
		private:
			static CORE::ConstantBuffer cBuffer;		// �o�b�t�@
			static void *matrix;						// �s��

		public:
			~GPU_ACCESS() { Release(); }

			/// <summary>
			/// ������
			/// </summary>	
			static void Initialize(void) {
				// �萔�o�b�t�@�����Ɠo�^�E�X�V
				cBuffer.Create(sizeof(T), Device3D::Get());
				Device3D::SetConstantBuffer(0, 1, cBuffer);
				matrix = new T;
				memset(matrix, 0, sizeof(T));
				Update();
			}

			/// <summary>
			/// ���
			/// </summary>	
			static void Release(void) {
				if (matrix) delete matrix;
				matrix = nullptr;
				cBuffer.Release();
			}

			/// <summary>
			/// �X�V
			/// </summary>	
			static void Update(void) {
				Device3D::UpdateConstantBuffer(matrix, cBuffer);
			}

			/// <summary>
			/// �擾
			/// </summary>
			static T *Get(void) { return (T *)matrix; }
		};
	}
}

#endif

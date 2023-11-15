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
* include宣言
------------------------------------------------------------------------------*/
#include "dx11_constant_buffer.h"
#include "dx11_device3d.h"
#include "dx_matrix.h"

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
* enum宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class宣言
------------------------------------------------------------------------------*/
namespace DX {
	namespace DX11 {
		template<class T>
		class GPU_ACCESS {
		private:
			static CORE::ConstantBuffer cBuffer;		// バッファ
			static void *matrix;						// 行列

		public:
			~GPU_ACCESS() { Release(); }

			/// <summary>
			/// 初期化
			/// </summary>	
			static void Initialize(void) {
				// 定数バッファ生成と登録・更新
				cBuffer.Create(sizeof(T), Device3D::Get());
				Device3D::SetConstantBuffer(0, 1, cBuffer);
				matrix = new T;
				memset(matrix, 0, sizeof(T));
				Update();
			}

			/// <summary>
			/// 解放
			/// </summary>	
			static void Release(void) {
				if (matrix) delete matrix;
				matrix = nullptr;
				cBuffer.Release();
			}

			/// <summary>
			/// 更新
			/// </summary>	
			static void Update(void) {
				Device3D::UpdateConstantBuffer(matrix, cBuffer);
			}

			/// <summary>
			/// 取得
			/// </summary>
			static T *Get(void) { return (T *)matrix; }
		};
	}
}

#endif

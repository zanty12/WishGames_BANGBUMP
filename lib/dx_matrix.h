/*==============================================================================

	Title		: MATRIX
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_MATRIX_H_
#define _DX11_MATRIX_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include <DirectXMath.h>
#include "rect.h"
#include "vector.h"

/*------------------------------------------------------------------------------
* define�錾
------------------------------------------------------------------------------*/
#define MATRIX_MUL(row, mat, column) _##row##1 * mat._##1##column + _##row##2 * mat._##2##column + _##row##3 * mat._##3##column + _##row##4 * mat._##4##column

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
	class MATRIX {
	private:
		union {
			float m[4][4] = {};
			struct {
				float _11;
				float _12;
				float _13;
				float _14;
				float _21;
				float _22;
				float _23;
				float _24;
				float _31;
				float _32;
				float _33;
				float _34;
				float _41;
				float _42;
				float _43;
				float _44;
			};
			struct v {
				float x, y, z, w;
			} v[4];
		};
		//DirectX::XMMATRIX matrix;			// �s��

	public:
		MATRIX() { }
		MATRIX(float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44) :
			_11(_11), _12(_12), _13(_13), _14(_14),
			_21(_21), _22(_22), _23(_23), _24(_24),
			_31(_31), _32(_32), _33(_33), _34(_34),
			_41(_41), _42(_42), _43(_43), _44(_44) { }

		/// <summary>
		/// �ˉe���W�s��ɂ���
		/// </summary>
		/// <param name="screenWidth"></param>
		/// <param name="screenHeight"></param>
		void SetWorldViewProjection(int screenWidth, int screenHeight);

		/// <summary>
		/// �ˉe���W�s��ɂ���
		/// </summary>
		void SetWorldViewProjection(Rect rect);

		/// <summary>
		/// �ˉe���W�s��ɂ���
		/// </summary>
		void SetWorldViewProjection3D(Rect rect);

		/// <summary>
		/// �g��k���s��
		/// </summary>
		/// <param name="scale"></param>
		void SetScaling(Vector3 scale);

		/// <summary>
		/// ��]�s��
		/// </summary>
		/// <param name="scale"></param>
		void SetRotation(Vector3 rotate);

		/// <summary>
		/// ���s�ړ��s��
		/// </summary>
		/// <param name="position"></param>
		void SetTranslation(Vector3 position);

		/// <summary>
		/// �t�s��
		/// </summary>
		/// <param name=""></param>
		void Transpose(void);


		MATRIX operator*(MATRIX mat);

	};
}

#endif
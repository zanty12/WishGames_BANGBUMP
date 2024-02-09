#include "dx_matrix.h"

namespace DX {
	void MATRIX::SetWorldViewProjection(int screenWidth, int screenHeight) {
		DirectX::XMMATRIX matrix = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, screenWidth, screenHeight, 0.0f, 0.0f, 1.0f);

		memcpy(this, &matrix, sizeof(matrix));
		// シェーダーバッファ用に行列を変換する（入れ替える）
		Transpose();
	}

	void MATRIX::SetWorldViewProjection(Rect rect) {
		ZeroMemory(this, sizeof(*this));
		// 射影座標
		{
			v[0].x = +2.0f / (rect.right - rect.left);
			v[1].y = +2.0f / (rect.top - rect.bottom);
			v[2].z = +1.0f;


			v[3].x = -1.0f;
			v[3].y = +1.0f;
			v[3].w = +1.0f;
		}

		// シェーダーバッファ用に行列を変換する（入れ替える）
		Transpose();
	}

	void MATRIX::SetWorldViewProjection3D(Rect rect) {
		ZeroMemory(this, sizeof(*this));
		float n = 00.0f;
		float f = 10000.0f;

		// 射影座標
		DirectX::XMMATRIX matrix = DirectX::XMMatrixPerspectiveFovLH(1.0f, 1920.0f / 1080.0f, 1.0f, 10000.0f);
		memcpy(this, &matrix, sizeof(matrix));
		//{
		//	v[0].x = +2.0f / (rect.right - rect.left);
		//	v[1].y = +2.0f / (rect.top - rect.bottom);
		//	v[2].z = f * (1.0f / (f - n));

		//	v[3].x = -1.0f;
		//	v[3].y = +1.0f;
		//	v[3].z = +1.0f;
		//	v[3].w = +1.0f;
		//}


		// シェーダーバッファ用に行列を変換する（入れ替える）
		Transpose();
	}

	void MATRIX::SetScaling(Vector3 scale) {
		ZeroMemory(this, sizeof(*this));

		// 拡大縮小
		{
			v[0].x = scale.x;
			v[1].y = scale.y;
			v[2].z = scale.z;
			v[3].w = 1.0f;
		}

		// シェーダーバッファ用に行列を変換する（入れ替える）
		Transpose();
	}

	void MATRIX::SetRotation(Vector3 rotate) {
		// 回転
		DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		memcpy(this, &matrix, sizeof(matrix));
		// シェーダーバッファ用に行列を変換する（入れ替える）
		Transpose();
	}

	void MATRIX::SetTranslation(Vector3 position) {
		// 移動
		DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		memcpy(this, &matrix, sizeof(matrix));

		// シェーダーバッファ用に行列を変換する（入れ替える）
		Transpose();
	}

	void MATRIX::Transpose(void) {
		MATRIX m = *this;
		_12 = m._21;
		_13 = m._31;
		_14 = m._41;

		_21 = m._12;
		_23 = m._32;
		_24 = m._42;

		_31 = m._13;
		_32 = m._23;
		_34 = m._43;

		_41 = m._14;
		_42 = m._24;
		_43 = m._34;
	}
	MATRIX MATRIX::operator*(MATRIX mat) {
		return {
			MATRIX_MUL(1, mat, 1), MATRIX_MUL(1, mat, 2), MATRIX_MUL(1, mat, 3), MATRIX_MUL(1, mat, 4),
			MATRIX_MUL(2, mat, 1), MATRIX_MUL(2, mat, 2), MATRIX_MUL(2, mat, 3), MATRIX_MUL(2, mat, 4),
			MATRIX_MUL(3, mat, 1), MATRIX_MUL(3, mat, 2), MATRIX_MUL(3, mat, 3), MATRIX_MUL(3, mat, 4),
			MATRIX_MUL(4, mat, 1), MATRIX_MUL(4, mat, 2), MATRIX_MUL(4, mat, 3), MATRIX_MUL(4, mat, 4),
		};
	}
}
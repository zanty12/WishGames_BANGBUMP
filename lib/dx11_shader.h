/*==============================================================================

	Title		: SHADER
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _DX11_SHADER_H_
#define _DX11_SHADER_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include <d3d.h>
#include <d3dcompiler.h>
#include <locale.h>
#include <memory>
#include "errormsg.h"


#pragma comment(lib, "d3dcompiler.lib")

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
	enum SHADER_VERSION {
		v1_0,
		v2_0,
		v3_0,
		v4_0,
		v5_0,
	};

	template <class T>
	class Shader {
	protected:
		T *shader = nullptr;
		ID3DBlob *blob = nullptr;

	public:

		~Shader() { Release(); }

		/// <summary>
		/// 作成
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		virtual HRESULT Create(ID3D11Device *device) = 0;

		/// <summary>
		/// シェーダーコンパイル
		/// </summary>
		/// <param name="path">シェーダーファイル</param>
		/// <param name="entry">エントリーポイント</param>
		/// <param name="version">バージョン</param>
		/// <returns></returns>
		HRESULT Compile(LPCWSTR path, char *entry, char *version) {
			CREATE_CHECK(shader);
			CREATE_CHECK(blob);
			HRESULT hr;

			ID3DBlob *errorBlob = NULL;

//#ifdef _DEBUG
			UINT flag = D3DCOMPILE_DEBUG;
			//flag |= D3DCOMPILE_DEBUG;
//#endif
			hr = D3DCompileFromFile(
				path,
				NULL,
				NULL,
				entry,
				version,
				flag,
				0,
				&blob,
				&errorBlob
			);

			if (FAILED(hr))
			{
				// マルチバイト文字列からワイド文字列へ変換
				setlocale(LC_CTYPE, "jpn");
				wchar_t wFilename[1024];
				size_t ret;
				mbstowcs_s(&ret, wFilename, (const char *)errorBlob->GetBufferPointer(), 1024);

				MESSAGE(L"Compile Error", wFilename);
				errorBlob->Release();
				return hr;
			}

			return hr;
		}

		/// <summary>
		/// ブロッブの取得
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		ID3DBlob *GetBlob(void) { return blob; }

		/// <summary>
		/// 解放
		/// </summary>
		/// <param name=""></param>
		void Release(void) {
			if (shader) shader->Release();
			if (blob) blob->Release();
			shader = nullptr;
			blob = nullptr;
		}

		operator T *() { return shader; }
		operator ID3DBlob *() { return blob; }
	};
}

#endif

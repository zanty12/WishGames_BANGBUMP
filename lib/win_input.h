/*==============================================================================

	Title		: WIN_INPUT
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _WIN_INPUT_H_
#define _WIN_INPUT_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include <windows.h>
#include <string.h>
#include "win_window.h"
#include "stdlib.h"

/*------------------------------------------------------------------------------
* define宣言
------------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS

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
namespace WIN {
	class Input {
	public:
		// https://learn.microsoft.com/ja-jp/windows/win32/inputdev/virtual-key-codes
		enum KeyCode {
			LEFT_MOUSE = 0x01,
			RIGHT_MOUSE = 0x02,
			SPACE = 0x20,
		};

	public:
		static bool keys[256];
		static bool olds[256];



	public:

		/// <summary>
		/// キー状態の取得
		/// </summary>
		/// <param name="key">キー</param>
		static bool GetKey(unsigned int key);

		/// <summary>
		/// キー状態の取得
		/// </summary>
		/// <param name="key">キー</param>
		static bool GetKeyUp(unsigned int key);

		/// <summary>
		/// キー状態の取得
		/// </summary>
		/// <param name="key">キー</param>
		static bool GetKeyDown(unsigned int key);

		/// <summary>
		/// マウス座標の取得
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		static Vector2Int GetMousePosition(void);

		/// <summary>
		/// マウス座標の取得
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		static Vector2Int GetMousePosition(HWND hwnd);

		/// <summary>
		/// キーバッファの更新
		/// </summary>
		static void Update(void);
	};
}

#endif

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
* include�錾
------------------------------------------------------------------------------*/
#include <windows.h>
#include <string.h>
#include "win_window.h"
#include "stdlib.h"

/*------------------------------------------------------------------------------
* define�錾
------------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS

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
		/// �L�[��Ԃ̎擾
		/// </summary>
		/// <param name="key">�L�[</param>
		static bool GetKey(unsigned int key);

		/// <summary>
		/// �L�[��Ԃ̎擾
		/// </summary>
		/// <param name="key">�L�[</param>
		static bool GetKeyUp(unsigned int key);

		/// <summary>
		/// �L�[��Ԃ̎擾
		/// </summary>
		/// <param name="key">�L�[</param>
		static bool GetKeyDown(unsigned int key);

		/// <summary>
		/// �}�E�X���W�̎擾
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		static Vector2Int GetMousePosition(void);

		/// <summary>
		/// �}�E�X���W�̎擾
		/// </summary>
		/// <param name=""></param>
		/// <returns></returns>
		static Vector2Int GetMousePosition(HWND hwnd);

		/// <summary>
		/// �L�[�o�b�t�@�̍X�V
		/// </summary>
		static void Update(void);
	};
}

#endif

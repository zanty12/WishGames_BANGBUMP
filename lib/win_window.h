/*==============================================================================

	Title		: WIN_WINDOW
	Description : �E�B���h�E
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _WIN_WINDOW_H_
#define _WIN_WINDOW_H_


/*------------------------------------------------------------------------------
* include�錾
------------------------------------------------------------------------------*/
#include <windows.h>
#include "win_input.h"
#include "stdlib.h"

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
namespace WIN {
	class Window {
	protected:
		WNDCLASSEX wcex;				// �E�B���h�E�N���X
		HWND hwnd = nullptr;			// �n���h���E�B���h�E
		bool isQuit = false;			// �I�����Ă��邩�ۂ�

	public:
		get<bool> IsQuit = isQuit;



	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
			Window *pThis = nullptr;

			// �I���W�i���E�B���h�E�v���V�[�W���̓o�^
			if (message == WM_CREATE) {
				CREATESTRUCT *pCreate = (CREATESTRUCT *)lParam;
				pThis = (Window *)pCreate->lpCreateParams;
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

				pThis->hwnd = hwnd;
			}
			// �擾
			else {
				pThis = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			}

			if (pThis) return pThis->Proc(message, wParam, lParam);
			else return DefWindowProc(hwnd, message, wParam, lParam);
		}

	public:

		void CreateWinClass(wchar_t *name, HINSTANCE hInstance);
		void CreateHwnd(wchar_t *className, wchar_t *name, UINT style, HINSTANCE hInstance, HWND hwnd, Vector2Int pos, Vector2Int size);

		/// <summary>
		/// �E�B���h�E�̍쐬
		/// </summary>
		/// <param name="className">�N���X��</param>
		/// <param name="name">�E�B���h�E��</param>
		/// <param name="hInstance">�n���h���C���X�^���X</param>
		/// <param name="WndProc">�E�B���h�E�v���V�[�W��</param>
		/// <param name="pos">���W</param>
		/// <param name="size">�T�C�Y</param>
		void Create(const wchar_t *className, const wchar_t *name, HINSTANCE hInstance, Vector2Int pos, Vector2Int size);

		/// <summary>
		/// �E�B���h�E�̍쐬
		/// </summary>
		/// <param name="className">�N���X��</param>
		/// <param name="name">�E�B���h�E��</param>
		/// <param name="style">�X�^�C��</param>
		/// <param name="hwnd">�e�n���h���E�B���h�E</param>
		/// <param name="hInstance">�n���h���C���X�^���X</param>
		/// <param name="WndProc">�E�B���h�E�v���V�[�W��</param>
		/// <param name="pos">���W</param>
		/// <param name="size">�T�C�Y</param>
		void Create(const wchar_t *className, const wchar_t *name, UINT style, HINSTANCE hInstance, HWND hwnd, Vector2Int pos, Vector2Int size);

		/// <summary>
		/// ���݂̃T�C�Y���擾
		/// </summary>
		/// <returns></returns>
		Vector2Int GetSize();

		/// <summary>
		/// ���݂̍��W���擾
		/// </summary>
		/// <returns></returns>
		Vector2Int GetPosition();

		/// <summary>
		/// ���݂̃��N�g���擾
		/// </summary>
		/// <returns></returns>
		Rect GetRect();

		/// <summary>
		/// HWND���擾
		/// </summary>
		/// <returns></returns>
		HWND &GetHwnd() { return hwnd; }

		/// <summary>
		/// �E�B���h�E�̕\��
		/// </summary>
		void Show();





		virtual LRESULT Proc(UINT message, WPARAM wParam, LPARAM lParam);

		operator HWND() { return hwnd; }
	};
}


#endif

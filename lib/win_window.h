/*==============================================================================

	Title		: WIN_WINDOW
	Description : ウィンドウ
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _WIN_WINDOW_H_
#define _WIN_WINDOW_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include <windows.h>
#include "win_input.h"
#include "stdlib.h"

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
namespace WIN {
	class Window {
	protected:
		WNDCLASSEX wcex;				// ウィンドウクラス
		HWND hwnd = nullptr;			// ハンドルウィンドウ
		bool isQuit = false;			// 終了しているか否か

	public:
		get<bool> IsQuit = isQuit;



	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
			Window *pThis = nullptr;

			// オリジナルウィンドウプロシージャの登録
			if (message == WM_CREATE) {
				CREATESTRUCT *pCreate = (CREATESTRUCT *)lParam;
				pThis = (Window *)pCreate->lpCreateParams;
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

				pThis->hwnd = hwnd;
			}
			// 取得
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
		/// ウィンドウの作成
		/// </summary>
		/// <param name="className">クラス名</param>
		/// <param name="name">ウィンドウ名</param>
		/// <param name="hInstance">ハンドルインスタンス</param>
		/// <param name="WndProc">ウィンドウプロシージャ</param>
		/// <param name="pos">座標</param>
		/// <param name="size">サイズ</param>
		void Create(const wchar_t *className, const wchar_t *name, HINSTANCE hInstance, Vector2Int pos, Vector2Int size);

		/// <summary>
		/// ウィンドウの作成
		/// </summary>
		/// <param name="className">クラス名</param>
		/// <param name="name">ウィンドウ名</param>
		/// <param name="style">スタイル</param>
		/// <param name="hwnd">親ハンドルウィンドウ</param>
		/// <param name="hInstance">ハンドルインスタンス</param>
		/// <param name="WndProc">ウィンドウプロシージャ</param>
		/// <param name="pos">座標</param>
		/// <param name="size">サイズ</param>
		void Create(const wchar_t *className, const wchar_t *name, UINT style, HINSTANCE hInstance, HWND hwnd, Vector2Int pos, Vector2Int size);

		/// <summary>
		/// 現在のサイズを取得
		/// </summary>
		/// <returns></returns>
		Vector2Int GetSize();

		/// <summary>
		/// 現在の座標を取得
		/// </summary>
		/// <returns></returns>
		Vector2Int GetPosition();

		/// <summary>
		/// 現在のレクトを取得
		/// </summary>
		/// <returns></returns>
		Rect GetRect();

		/// <summary>
		/// HWNDを取得
		/// </summary>
		/// <returns></returns>
		HWND &GetHwnd() { return hwnd; }

		/// <summary>
		/// ウィンドウの表示
		/// </summary>
		void Show();





		virtual LRESULT Proc(UINT message, WPARAM wParam, LPARAM lParam);

		operator HWND() { return hwnd; }
	};
}


#endif

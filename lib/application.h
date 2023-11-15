/*==============================================================================

	Title		: APPLICATION
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _APPLICATION_H_
#define _APPLICATION_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include <string>
#include <windows.h>
#include "cluster.h"
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
namespace APP {
	class Application : public Behavior {
	private:
		static Cluster appList;										// アプリケーションのリスト

		static DWORD framelate;										// フレームレート
		static DWORD deltaTime;										// デルタタイム
		static int fps;												// FPS

	public:
		std::string name = "";										// 名前
		HWND hwnd = nullptr;										// ウィンドウ
		get<Cluster> app = appList;


	public:

		Application() : Behavior("Application") { }

		/// <summary>
		/// アプリケーションの登録
		/// </summary>
		/// <param name="data">アプリケーション</param>
		template<class T, class... Args>
		static T *Create(Args... args) {
			T* app = appList.Create<T>(args...);
			return app;
		}

		/// <summary>
		/// 実行
		/// </summary>
		static void Run();

		/// <summary>
		/// デルタタイムの取得
		/// </summary>
		/// <returns></returns>
		static float GetDeltaTime(void) { return deltaTime; }

		/// <summary>
		/// fpsの取得
		/// </summary>
		/// <returns></returns>
		static float GetFps(void) { return fps; }
	};
}

#endif

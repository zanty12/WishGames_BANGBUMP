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
* include�錾
------------------------------------------------------------------------------*/
#include <string>
#include <windows.h>
#include "cluster.h"
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
namespace APP {
	class Application : public Behavior {
	private:
		static Cluster appList;										// �A�v���P�[�V�����̃��X�g

		static DWORD framelate;										// �t���[�����[�g
		static DWORD deltaTime;										// �f���^�^�C��
		static int fps;												// FPS

	public:
		std::string name = "";										// ���O
		HWND hwnd = nullptr;										// �E�B���h�E
		get<Cluster> app = appList;


	public:

		Application() : Behavior("Application") { }

		/// <summary>
		/// �A�v���P�[�V�����̓o�^
		/// </summary>
		/// <param name="data">�A�v���P�[�V����</param>
		template<class T, class... Args>
		static T *Create(Args... args) {
			T* app = appList.Create<T>(args...);
			return app;
		}

		/// <summary>
		/// ���s
		/// </summary>
		static void Run();

		/// <summary>
		/// �f���^�^�C���̎擾
		/// </summary>
		/// <returns></returns>
		static float GetDeltaTime(void) { return deltaTime; }

		/// <summary>
		/// fps�̎擾
		/// </summary>
		/// <returns></returns>
		static float GetFps(void) { return fps; }
	};
}

#endif

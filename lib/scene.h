///*==============================================================================
//
//	Title		: SCENE
//	Description :
//	Date		:
//	Author		:
//
//--------------------------------------------------------------------------------
//
//==============================================================================*/
//
//
//
//#pragma once
//#ifndef _SCENE_H_
//#define _SCENE_H_
//
//
///*------------------------------------------------------------------------------
//* include宣言
//------------------------------------------------------------------------------*/
//#include <windows.h>
//#include "behavior.h"
//#include "object.h"
//#include "transform.h"
//#include "component.h"
//#include "stdlib.h"
//
///*------------------------------------------------------------------------------
//* define宣言
//------------------------------------------------------------------------------*/
//
//
///*------------------------------------------------------------------------------
//* extern宣言
//------------------------------------------------------------------------------*/
//
//
///*------------------------------------------------------------------------------
//* prototype宣言
//------------------------------------------------------------------------------*/
//
//
///*------------------------------------------------------------------------------
//* enum宣言
//------------------------------------------------------------------------------*/
//
//
///*------------------------------------------------------------------------------
//* class宣言
//------------------------------------------------------------------------------*/
//namespace APP {
//	namespace WORLD {
//		class Object;
//		class Transform;
//		class Component;
//
//		class Scene : public Behavior {
//		private:
//			const float maxDistance = 100;		// 空間の最大距離
//			const int divNum = 3;				// 空間の最大分割数
//			Scene *spaces[8] = {};				// 子空間
//
//		public:
//			friend Object;
//			friend Transform;
//			friend Component;
//
//
//		private:
//
//			/// <summary>
//			/// 座標をインデックス番号にする
//			/// </summary>
//			DWORD to_index(Vector3 position);
//
//			/// <summary>
//			/// 座標をインデックス番号にする
//			/// </summary>
//			DWORD to_index(Vector3 position1, Vector3 position2);
//
//			/// <summary>
//			/// 空間のオブジェクトリストを取得する
//			/// </summary>
//			/// <param name="index"></param>
//			/// <returns></returns>
//			Behavior *dive(DWORD index, int loop);
//
//		protected:
//			/// <summary>
//			/// 空間を移動する
//			/// </summary>
//			void scene_move(Object &object);
//
//		public:
//			Scene() : Behavior("Scene") { }
//
//			/// <summary>
//			/// 追加
//			/// </summary>
//			template<class T, class... Args>
//			T *Create(Args... args) {
//				// インスタンスを生成する
//				T *instance = Add<T>(std::forward<Args>(args)...);
//				Object *object = instance;
//				object->pScene = this;
//				return instance;
//			}
//
//			/// <summary>
//			/// 追加
//			/// </summary>
//			Object *Create(void);
//		};
//	}
//}
//
//#endif

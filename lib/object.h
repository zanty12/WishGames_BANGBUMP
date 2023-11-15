/*==============================================================================

	Title		: OBJECT
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _OBJECT_H_
#define _OBJECT_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include "cluster.h"
#include "component.h"
#include "transform.h"
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
	namespace WORLD {
		class Scene;
		class Transform;
		class Component;

		class Object : public Behavior {
		private:
			Cluster *&components = child;				// コンポーネント集団
			
		protected:
			DWORD memberIndex = 0;						// 空間インデックス
			Scene *pScene = nullptr;					// シーン

		public:
			Transform transform;						// トランスフォーム
			get<Scene*> scene = pScene;					// シーン
			
			friend Scene;
			friend Transform;
			friend Component;



		public:
			Object() : Behavior("Object") { child = new Cluster(); }
			Object(int priority) : Behavior("Object", priority) { child = new Cluster(); }

			/// <summary>
			/// コンポーネントを追加する
			/// </summary>
			template<class T, class... Args>
			T *AddComponent(Args... args) {
				T *instance = components->Create<T>(std::forward<Args>(args)...);
				Component *component = instance;
				component->pScene = pScene;
				component->pObject = this;
				return instance;
			}

			/// <summary>
			/// コンポーネントを取得する
			/// </summary>
			template<class T>
			T *GetComponent(std::string type) {
				return (T*)child->Get(type);
			}

			/// <summary>
			/// 解放
			/// </summary>
			void Release(void) { if (components) components->Release(); }

			virtual void OnTriggerEnter(Object *object) { };
			virtual void OnTriggerStay(Object *object) { };
			virtual void OnTriggerExit(Object *object) { };
		};
	}
}

#endif

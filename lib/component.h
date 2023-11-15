/*==============================================================================

	Title		: COMPONENT
	Description :
	Date		:
	Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _COMPONENT_H_
#define _COMPONENT_H_


/*------------------------------------------------------------------------------
* include宣言
------------------------------------------------------------------------------*/
#include "cluster.h"
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
* enum宣言
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class宣言
------------------------------------------------------------------------------*/
namespace APP {
	namespace WORLD {
		class Scene;
		class Object;
		class Transform;

		class Component : public Behavior {
		protected:
			Scene *pScene = nullptr;							// シーン
			Object* pObject = nullptr;							// オブジェクト

		public:
			get<Scene*> scene = pScene;							// シーン
			get<Object*> object = pObject;						// オブジェクト

			friend Scene;
			friend Object;
			friend Transform;


		public:
			Component() : Behavior("Component") { }
			Component(const char* name) : Behavior(name, 0) { }
			Component(const char* name, int priority) : Behavior(name, priority) { }

			virtual void OnTriggerEnter(Object *object) { };
			virtual void OnTriggerStay(Object *object) { };
			virtual void OnTriggerExit(Object *object) { };
		};
	}
}

#endif



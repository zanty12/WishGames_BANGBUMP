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
* include�錾
------------------------------------------------------------------------------*/
#include "cluster.h"
#include "transform.h"
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
* enum�錾
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* class�錾
------------------------------------------------------------------------------*/
namespace APP {
	namespace WORLD {
		class Scene;
		class Object;
		class Transform;

		class Component : public Behavior {
		protected:
			Scene *pScene = nullptr;							// �V�[��
			Object* pObject = nullptr;							// �I�u�W�F�N�g

		public:
			get<Scene*> scene = pScene;							// �V�[��
			get<Object*> object = pObject;						// �I�u�W�F�N�g

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



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
* include�錾
------------------------------------------------------------------------------*/
#include "cluster.h"
#include "component.h"
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
* class�錾
------------------------------------------------------------------------------*/
namespace APP {
	namespace WORLD {
		class Scene;
		class Transform;
		class Component;

		class Object : public Behavior {
		private:
			Cluster *&components = child;				// �R���|�[�l���g�W�c
			
		protected:
			DWORD memberIndex = 0;						// ��ԃC���f�b�N�X
			Scene *pScene = nullptr;					// �V�[��

		public:
			Transform transform;						// �g�����X�t�H�[��
			get<Scene*> scene = pScene;					// �V�[��
			
			friend Scene;
			friend Transform;
			friend Component;



		public:
			Object() : Behavior("Object") { child = new Cluster(); }
			Object(int priority) : Behavior("Object", priority) { child = new Cluster(); }

			/// <summary>
			/// �R���|�[�l���g��ǉ�����
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
			/// �R���|�[�l���g���擾����
			/// </summary>
			template<class T>
			T *GetComponent(std::string type) {
				return (T*)child->Get(type);
			}

			/// <summary>
			/// ���
			/// </summary>
			void Release(void) { if (components) components->Release(); }

			virtual void OnTriggerEnter(Object *object) { };
			virtual void OnTriggerStay(Object *object) { };
			virtual void OnTriggerExit(Object *object) { };
		};
	}
}

#endif

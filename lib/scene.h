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
//* include�錾
//------------------------------------------------------------------------------*/
//#include <windows.h>
//#include "behavior.h"
//#include "object.h"
//#include "transform.h"
//#include "component.h"
//#include "stdlib.h"
//
///*------------------------------------------------------------------------------
//* define�錾
//------------------------------------------------------------------------------*/
//
//
///*------------------------------------------------------------------------------
//* extern�錾
//------------------------------------------------------------------------------*/
//
//
///*------------------------------------------------------------------------------
//* prototype�錾
//------------------------------------------------------------------------------*/
//
//
///*------------------------------------------------------------------------------
//* enum�錾
//------------------------------------------------------------------------------*/
//
//
///*------------------------------------------------------------------------------
//* class�錾
//------------------------------------------------------------------------------*/
//namespace APP {
//	namespace WORLD {
//		class Object;
//		class Transform;
//		class Component;
//
//		class Scene : public Behavior {
//		private:
//			const float maxDistance = 100;		// ��Ԃ̍ő勗��
//			const int divNum = 3;				// ��Ԃ̍ő啪����
//			Scene *spaces[8] = {};				// �q���
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
//			/// ���W���C���f�b�N�X�ԍ��ɂ���
//			/// </summary>
//			DWORD to_index(Vector3 position);
//
//			/// <summary>
//			/// ���W���C���f�b�N�X�ԍ��ɂ���
//			/// </summary>
//			DWORD to_index(Vector3 position1, Vector3 position2);
//
//			/// <summary>
//			/// ��Ԃ̃I�u�W�F�N�g���X�g���擾����
//			/// </summary>
//			/// <param name="index"></param>
//			/// <returns></returns>
//			Behavior *dive(DWORD index, int loop);
//
//		protected:
//			/// <summary>
//			/// ��Ԃ��ړ�����
//			/// </summary>
//			void scene_move(Object &object);
//
//		public:
//			Scene() : Behavior("Scene") { }
//
//			/// <summary>
//			/// �ǉ�
//			/// </summary>
//			template<class T, class... Args>
//			T *Create(Args... args) {
//				// �C���X�^���X�𐶐�����
//				T *instance = Add<T>(std::forward<Args>(args)...);
//				Object *object = instance;
//				object->pScene = this;
//				return instance;
//			}
//
//			/// <summary>
//			/// �ǉ�
//			/// </summary>
//			Object *Create(void);
//		};
//	}
//}
//
//#endif

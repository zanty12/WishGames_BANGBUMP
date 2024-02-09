//#include "scene.h"
//
//
//namespace APP {
//	namespace WORLD {
//		DWORD Scene::to_index(Vector3 position) {
//			// 1�P�ʓ�����̋���
//			float unitDistance = maxDistance / (1 << (divNum));
//			DWORD xIndex = position.x / unitDistance;
//			DWORD yIndex = position.y / unitDistance;
//			DWORD zIndex = position.z / unitDistance;
//
//			DWORD index = 0;
//			for (int i = 0; i < divNum; i++) {
//				index |= (zIndex >> (divNum - i - 1)) & 0x01;
//				index = index << 1;
//				index |= (yIndex >> (divNum - i - 1)) & 0x01;
//				index = index << 1;
//				index |= (xIndex >> (divNum - i - 1)) & 0x01;
//				if (i != divNum - 1) index = index << 1;
//			}
//
//			return index;
//		}
//
//		DWORD Scene::to_index(Vector3 position1, Vector3 position2) {
//			if (position2.x < position1.x ||
//				position2.y < position1.y ||
//				position2.z < position1.z) return -1;
//
//			// ���W���C���f�b�N�X�ɕϊ�����
//			DWORD index1 = to_index(position1);
//			DWORD index2 = to_index(position2);
//
//			int convert = index1 ^ index2;
//			DWORD index = index1;
//
//			// �r�b�g�𒲂ׂ�
//			for (int i = 0; i < divNum; i++) {
//				int shift = (divNum - i - 1) * 3;
//				index |= index1 & (0x07 << divNum);
//
//
//				// �q�G�����L�[��Ԃɑ����Ă���Ȃ�A�I��
//				if (convert >> shift) break;
//			}
//
//			return index;
//		}
//
//		Behavior *Scene::dive(DWORD index, int loop) {
//			// ��Ԃɂ��ǂ蒅�����Ȃ�A�I��
//			if (!index) return head;
//
//			// ��Ԃ̗v�f�ԍ��𒲂ׂ�
//			int shift = (loop - 1) * 3;
//			int i = (index >> shift);
//			// �����ׂ�悤�Ńr�b�g�����炷
//			index = index & ~(0x07 << shift);
//
//			if (spaces[i] == nullptr) spaces[i] = new Scene();
//			return spaces[i]->dive(index, loop - 1);
//		}
//
//		void Scene::scene_move(Object &object) {
//			// �C���f�b�N�X���擾
//			DWORD index = to_index(object.transform.position);
//
//			// �ړ����Ă��Ȃ��Ȃ�A�I��
//			if (index == object.memberIndex) return;
//			// ��Ԃ��ړ�����
//			//dive(object.memberIndex, divNum).Pop(object.id);
//			//dive(index, divNum).Push(&object);
//			//object.memberIndex = index;
//		}
//
//
//
//		Object *Scene::Create() {
//			Object *pObject = Add<Object>();
//			pObject->pScene = this;
//			return pObject;
//		}
//	}
//}

//#include "scene.h"
//
//
//namespace APP {
//	namespace WORLD {
//		DWORD Scene::to_index(Vector3 position) {
//			// 1単位当たりの距離
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
//			// 座標をインデックスに変換する
//			DWORD index1 = to_index(position1);
//			DWORD index2 = to_index(position2);
//
//			int convert = index1 ^ index2;
//			DWORD index = index1;
//
//			// ビットを調べる
//			for (int i = 0; i < divNum; i++) {
//				int shift = (divNum - i - 1) * 3;
//				index |= index1 & (0x07 << divNum);
//
//
//				// ヒエラルキー空間に属しているなら、終了
//				if (convert >> shift) break;
//			}
//
//			return index;
//		}
//
//		Behavior *Scene::dive(DWORD index, int loop) {
//			// 空間にたどり着いたなら、終了
//			if (!index) return head;
//
//			// 空間の要素番号を調べる
//			int shift = (loop - 1) * 3;
//			int i = (index >> shift);
//			// 次調べるようでビットをずらす
//			index = index & ~(0x07 << shift);
//
//			if (spaces[i] == nullptr) spaces[i] = new Scene();
//			return spaces[i]->dive(index, loop - 1);
//		}
//
//		void Scene::scene_move(Object &object) {
//			// インデックスを取得
//			DWORD index = to_index(object.transform.position);
//
//			// 移動していないなら、終了
//			if (index == object.memberIndex) return;
//			// 空間を移動する
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

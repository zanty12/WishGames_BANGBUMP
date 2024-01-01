#pragma once
#include <map>
#include "multi_header.h"
#include "multi_client_gameobject.h"

class MultiRenderer {
private:
	ClientGameObject object;
	std::map<int, Animator *> objects;
	struct {
		int playerTexNo = LoadTexture("data/texture/player.png");
		int enemy1TexNo = LoadTexture("data/texture/enemy1.png");
		int enemy2TexNo = LoadTexture("data/texture/enemy2.png");
		int enemy3TexNo = LoadTexture("data/texture/enemy3.png");
		int skillorbTexNo = LoadTexture("data/texture/skillorb.png");
	};


private:
	void DrawObject(OBJECT_DATA_CLIENT_SIDE &object) {
		int id = object.id;

		// �I�u�W�F�N�gID�𒲂ׂ�
		auto iterator = objects.find(id);

		// ���݂��Ȃ��Ȃ�o�^
		if (iterator == objects.end()) {
			Animator* anim = new Animator(&this->object);
			int texNo = 0;

			// �e�N�X�`���ԍ����w��
			switch (object.tag) {
			case OBJECT_DATA_CLIENT_SIDE::ENEMY: texNo = enemy1TexNo; break;
			case OBJECT_DATA_CLIENT_SIDE::SKILL_POINT: texNo = skillorbTexNo; break;
			}
			anim->SetTexNo(texNo);

			objects[id] = anim;
		}
		// �I�u�W�F�N�g�̍X�V
		else {
			Animator *anim = iterator->second;
			anim->SetPos(object.position);
			anim->SetRot(object.rotation);
			anim->SetScale(object.scale);

			anim->Draw();
		}
	}

public:
	void Draw(RESPONSE_PLAYER &res) {
		for (auto &object : res.objects) {
			DrawObject(object);
		}
	}
};
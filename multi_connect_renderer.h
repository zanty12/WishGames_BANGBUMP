#pragma once
#include <map>
#include "multi_header.h"
#include "multi_client_gameobject.h"
#include "asset.h"

class MultiRenderer {
private:
	ClientGameObject object;
	std::map<int, Animator *> objects;
	struct {
		int playerTexNo = LoadTexture(Asset::textures_.at(textures::player));
		int enemy1TexNo = LoadTexture("data/texture/enemy1.png");
		int enemy2TexNo = LoadTexture("data/texture/enemy2.png");
		int enemy3TexNo = LoadTexture("data/texture/enemy3.png");
		int skillorbTexNo = LoadTexture(Asset::textures_.at(textures::skill_orb));
	};


private:
	void DrawObject(Vector2 playerPosition, OBJECT_DATA_CLIENT_SIDE &object) {
		int id = object.id;

		// �I�u�W�F�N�gID�𒲂ׂ�
		auto iterator = objects.find(id);

		// ���݂��Ȃ��Ȃ�o�^
		if (iterator == objects.end()) {
			Animator* anim = new Animator(&this->object);
			int texNo = 0;

			// �e�N�X�`���ԍ����w��
			switch (object.tag) {
			case OBJECT_DATA_CLIENT_SIDE::PLAYER: texNo = playerTexNo; break;
			case OBJECT_DATA_CLIENT_SIDE::ENEMY1: texNo = enemy1TexNo; break;
			case OBJECT_DATA_CLIENT_SIDE::ENEMY2: texNo = enemy2TexNo; break;
			case OBJECT_DATA_CLIENT_SIDE::ENEMY3: texNo = enemy3TexNo; break;
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
			anim->SetTexenum(object.texture_enum);

			//anim->Draw();
		}
	}

public:
	void Draw(RESPONSE_PLAYER &res) {
		for (auto &client : res.clients) {
			OBJECT_DATA_CLIENT_SIDE data = {
				-client.id,							// ID�i�N���C�A���g��-�̒l�ɂ���j
				OBJECT_DATA_CLIENT_SIDE::PLAYER,	// tag
				OBJECT_DATA_CLIENT_SIDE::IDEL,		// �A�j���[�V����
				client.position,					// ���W
				0.0f,								// ��]
				Vector2::One * 50					// �g�k
			};
			DrawObject(res.clients.begin()->position, data);
		}
		for (auto &object : res.objects) {
			DrawObject(res.clients.begin()->position, object);
		}
	}
};
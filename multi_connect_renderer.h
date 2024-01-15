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

		// オブジェクトIDを調べる
		auto iterator = objects.find(id);

		// 存在しないなら登録
		if (iterator == objects.end()) {
			Animator* anim = new Animator(&this->object);
			int texNo = 0;

			// テクスチャ番号を指定
			switch (object.tag) {
			case OBJECT_DATA_CLIENT_SIDE::ENEMY: texNo = enemy1TexNo; break;
			case OBJECT_DATA_CLIENT_SIDE::SKILL_POINT: texNo = skillorbTexNo; break;
			}
			anim->SetTexNo(texNo);

			objects[id] = anim;
		}
		// オブジェクトの更新
		else {
			Animator *anim = iterator->second;
			anim->SetPos(object.position);
			anim->SetRot(object.rotation);
			anim->SetScale(object.scale);

			//anim->Draw();
		}
	}

public:
	void Draw(RESPONSE_PLAYER &res) {
		for (auto &object : res.objects) {
			DrawObject(res.clients.begin()->position, object);
		}
	}
};
#include "multi_skillorb.h"
#include "multiplay.h"

void ServerSkillOrb::Initialize(void) {
	radius = 50.0f;
}

void ServerSkillOrb::Loop(void) {
	for (auto &kvp : MultiPlayServer::clients_) {
		auto &player = kvp.second.player_;
		Vector2 direction = transform.position - player->transform.position;
		float distance = direction.Distance();

		if (distance <= radius + player->radius) {
			auto &player = kvp.second.player_;
			player->skillPoint += addPoint;
			Destroy();
		}
	}
}


ClientSkillOrb::ClientSkillOrb(Vector2 scale) {
	texNo = LoadTexture(Asset::textures_.at(textures::skill_orb));
	transform.scale = scale;
}

void ClientSkillOrb::Loop(void) {
	if (!isShow) return;
	DrawSprite(texNo, transform.position - MultiPlayClient::offset, 0.0f, transform.scale, Color::White);
	isShow = false;
}

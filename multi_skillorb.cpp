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
			auto &client = kvp.second;
			client.skillPoint++;
			Destroy();
		}
	}
}

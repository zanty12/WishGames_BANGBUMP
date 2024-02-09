#include "multi_skillorb.h"
#include "multiplay.h"
#pragma comment(lib, "lib/debug_lib.lib")

void ServerSkillOrb::Initialize(void) {
	timer.Start();
	radius = 50.0f;
}

void ServerSkillOrb::Loop(void) {

	velocity *= friction;
	transform.position += velocity;

	// �x�N�g����1�����Ȃ瓮���Ȃ��悤�ɂ���
	float magnitudeSq = velocity.DistanceSq();
	if (magnitudeSq <= 1) {
		velocity = Vector2::Zero;
		magnitudeSq = 0.0f;
	}

	// �Փ˔���
	if (magnitudeSq) MultiPlayServer::GetGameMode()->GetMap()->Collision(transform.position, radius);

	// ��b�ȓ��Ȃ�X�L���|�C���g���Ƃ�Ȃ�
	if (timer.GetNowTime() < 1000) return;


	bool  isDestroy = false;
	for (auto &kvp : MultiPlayServer::clients_) {
		// �폜�Ȃ牽�����Ȃ�
		if (isDestroy) return;


		auto &player = kvp.second.player_;
		Vector2 direction = transform.position - player->transform.position;
		float distance = direction.Distance();

		if (distance <= radius + player->radius) {
			auto &player = kvp.second.player_;
			// �|�C���g���Z
			player->skillPoint += addPoint;
			// �폜
			Destroy();
			isDestroy = true;
		}
	}
}


ClientSkillOrb::ClientSkillOrb(Vector2 scale) {
	transform.scale = scale;
}
void ClientSkillOrb::Loop(void) {
	if (!isShow) return;
	anim.Draw(transform.position - MultiPlayClient::offset, 0.0f, transform.scale, Color::White);
	isShow = false;
}

void ClientSkillOrb::Release(void) {
	// ���ʉ�
	PlaySE(haveSeNo, Vector2(Graphical::GetWidth(), Graphical::GetHeight()) * 0.5f + MultiPlayClient::offset, transform.position);
}

ClientSkillOrbSmall::ClientSkillOrbSmall() : ClientSkillOrb(Vector2::One * 80) { 
	texNo = LoadTexture("data/texture/Effect/effect_skillorb.png");
	anim = MultiAnimator(texNo, 5, 6, 0, 29, true);
};
ClientSkillOrbMidium::ClientSkillOrbMidium() : ClientSkillOrb(Vector2::One * 100) {
	texNo = LoadTexture("data/texture/Effect/effect_skillorb2.png");
	anim = MultiAnimator(texNo, 5, 6, 0, 29, true);
};
ClientSkillOrbBig::ClientSkillOrbBig() : ClientSkillOrb(Vector2::One * 120) {
	texNo = LoadTexture("data/texture/Effect/effect_skillorb3.png");
	anim = MultiAnimator(texNo, 5, 6, 0, 29, true);
};

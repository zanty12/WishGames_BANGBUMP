#include "multi_player.h"
#include "xinput.h"
#include "sprite.h"
#include "multiplay.h"

void ServerPlayer::Loop(void) {
	if (moveAttribute) moveAttribute->Move();
	if (attackAttribute) attackAttribute->Attack();

	transform.position += velocity;
}



void ClientPlayer::Loop(void) {
	DrawSprite(texNo, transform.position /*- MultiPlayClient::offset*/, 0.0f, transform.scale, Color::White);
}

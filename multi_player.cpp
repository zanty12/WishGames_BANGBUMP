#include "multi_player.h"
#include "xinput.h"

void ServerPlayer::Loop(void) {
	if (moveAttribute) moveAttribute->Move();
	if (attackAttribute) attackAttribute->Attack();

	transform.position += velocity;
}

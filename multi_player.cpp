#include "multi_player.h"
#include "xinput.h"

void ServerPlayer::Update(void) {
	if (moveAttribute) moveAttribute->Move();
	if (attackAttribute) attackAttribute->Attack();

	transform.position += velocity;
}

#include "multi_player.h"
#include "xinput.h"

void ServerPlayer::Update(void) {
	//if (moveAttribute) moveAttribute->Move();
	//if (attackAttribute) attackAttribute->Attack();

	velocity = Input::GetStickLeft(0) * 10.0f;
	transform.position += velocity;
}

#include "multi_player.h"
#include "xinput.h"

void ServerPlayer::Update(void) {
	velocity = Input::GetStickLeft(0) * 10;
}

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
	// アニメーションが切り替わった瞬間、アニメーションする位置を更新する
	if(preAnimType != animType) MultiAnimator::GetPlayer(animType, moveAttribute, attackAttribute, &anim);
	
	if (0.0 < velocity.x) isReverseX = true;
	else if (velocity.x < 0.0f) isReverseX = false;

	// 描画する
	anim.Draw(transform.position - MultiPlayClient::offset, transform.rotation, transform.scale * 3.0f, Color::White, isReverseX);

	// アニメーションタイプの更新
	preAnimType = animType;
}

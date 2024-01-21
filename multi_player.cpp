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
	// �A�j���[�V�������؂�ւ�����u�ԁA�A�j���[�V��������ʒu���X�V����
	if(preAnimType != animType) MultiAnimator::GetPlayer(animType, moveAttribute, attackAttribute, &anim);
	
	if (0.0 < velocity.x) isReverseX = true;
	else if (velocity.x < 0.0f) isReverseX = false;

	// �`�悷��
	anim.Draw(transform.position - MultiPlayClient::offset, transform.rotation, transform.scale * 3.0f, Color::White, isReverseX);

	// �A�j���[�V�����^�C�v�̍X�V
	preAnimType = animType;
}

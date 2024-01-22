#include "multi_player.h"
#include "xinput.h"
#include "sprite.h"
#include "multiplay.h"
#include "multi_skillorb.h"

void ServerPlayer::Loop(void) {
	if (moveAttribute) moveAttribute->Move();
	if (attackAttribute) attackAttribute->Attack();

	transform.position += velocity;
}

void ServerPlayer::Damage(ServerAttack *attack) {
	// �U���҂������Ȃ�I��
	if (attack->GetSelf() == this) return;

	SkillOrbDrop(attack->drop);
}

void ServerPlayer::SkillOrbDrop(int drop) {
	// �}�b�v���Ȃ��Ȃ�I��
	if (!map) return;	

	while (0 < drop) {
		// �h���b�v����l
		int tmpDrop = 0;

		// �X�L���|�C���g�i��j
		if (skillPoint <= ServerSkillOrbBig::AddPoint) {
			map->GetSkillOrbs()->Add<ServerSkillOrbBig>(Transform(transform.position));
			tmpDrop = ServerSkillOrbBig::AddPoint;
		}
		// �X�L���|�C���g�i���j
		else if (skillPoint <= ServerSkillOrbMidium::AddPoint) {
			map->GetSkillOrbs()->Add<ServerSkillOrbMidium>(Transform(transform.position));
			tmpDrop = ServerSkillOrbMidium::AddPoint;
		}
		// �X�L���|�C���g�i���j
		else if (skillPoint <= ServerSkillOrbSmall::AddPoint) {
			map->GetSkillOrbs()->Add<ServerSkillOrbSmall>(Transform(transform.position));
			tmpDrop = ServerSkillOrbSmall::AddPoint;
		}
		else return;

		// �h���b�v����
		if (skillPoint <= tmpDrop && drop <= tmpDrop) {
			skillPoint -= tmpDrop;
			drop -= tmpDrop;
		}
		else return;
	}

}



ClientPlayer::ClientPlayer(ATTRIBUTE_TYPE moveAttributeType, ATTRIBUTE_TYPE attackAttributeType, Transform transform) :
	moveAttribute(ClientAttribute::Create(this, moveAttributeType)), 
	attackAttribute(ClientAttribute::Create(this, attackAttributeType)) ,
	ClientMovableGameObject(transform) {	
	anim = MultiAnimator::GetPlayerInitialize(0, moveAttribute->GetAttribute(), attackAttribute->GetAttribute());
}

void ClientPlayer::Loop(void) {
	// �������Ȃ��Ȃ����
	if (!moveAttribute || !attackAttribute) return;


	// �ړ��A�j���[�V����
	if (animType == ANIMATION_TYPE_MOVE) {
		moveAttribute->Move();
	}
	// �U���A�j���[�V����
	if (animType == ANIMATION_TYPE_ATTACK) {
		attackAttribute->Attack();
	}

	// �A�j���[�V�������؂�ւ�����u�ԁA�A�j���[�V��������ʒu���X�V����
	if (preAnimType != animType) MultiAnimator::GetPlayer(animType, moveAttribute->GetAttribute(), attackAttribute->GetAttribute(), &anim);
	
	if (0.0 < velocity.x) isReverseX = true;
	else if (velocity.x < 0.0f) isReverseX = false;

	// �`�悷��
	anim.Draw(transform.position - MultiPlayClient::offset, transform.rotation, transform.scale * 3.0f, Color::White, isReverseX);

	// �A�j���[�V�����^�C�v�̍X�V
	preAnimType = animType;
}


#include "multi_player.h"
#include "xinput.h"
#include "sprite.h"
#include "multiplay.h"
#include "multi_skillorb.h"

void ServerPlayer::Loop(void) {
	if (moveAttribute) moveAttribute->Move();
	if (attackAttribute) attackAttribute->Attack();

	// ��������񂱃V�X�e��
	if (0.75f < Input::GetTriggerLeft(0) && 0.75f < Input::GetTriggerRight(0)) {
		ServerAttribute *tmp = attackAttribute;
		attackAttribute = moveAttribute;
		moveAttribute = tmp;
	}

	// �ړ�������
	transform.position += velocity + blownVelocity;

	// ������΂��̑��x������������
	blownVelocity *= blownFriction;
	if (blownVelocity.DistanceSq() < 1.0f) blownVelocity = Vector2::Zero;
}

void ServerPlayer::Damage(AttackServerSide *attack) {
	// �U���҂������Ȃ�I��
	if (attack->GetSelf() == this) return;

	SkillOrbDrop(attack->atkDrop);
}

void ServerPlayer::SkillOrbDrop(int drop) {
	// �}�b�v���Ȃ��Ȃ�I��
	if (!map) return;

	// �h���b�v����ő�l���v�Z����
	int maxDrop = skillPoint < drop ? skillPoint : drop;

	// �X�L���I�[�u���h���b�v������
	map->DropSkillOrb(maxDrop, transform.position, 10.0f);

	skillPoint -= maxDrop;
}



ClientPlayer::ClientPlayer(ATTRIBUTE_TYPE moveAttributeType, ATTRIBUTE_TYPE attackAttributeType, Transform transform) :
	moveAttribute(ClientAttribute::Create(this, moveAttributeType)), 
	attackAttribute(ClientAttribute::Create(this, attackAttributeType)) ,
	ClientMovableGameObject(transform) {	
	anim = MultiAnimator::GetPlayerInitialize(0, moveAttribute->GetAttribute(), attackAttribute->GetAttribute());
	this->transform.scale = Vector2::One * 150;
}

void ClientPlayer::Loop(void) {
	// �������Ȃ��Ȃ����
	if (!moveAttribute || !attackAttribute) return;


	// �ړ��A�j���[�V����
	if (animType == ANIMATION_TYPE_MOVE || animType == ANIMATION_TYPE_MOVE_CHARGE) {
		moveAttribute->Move();
	}
	// �U���A�j���[�V����
	if (animType == ANIMATION_TYPE_ATTACK || animType == ANIMATION_TYPE_ATTACK_CHARGE) {
		attackAttribute->Attack();
	}

	// �A�j���[�V�������؂�ւ�����u�ԁA�A�j���[�V��������ʒu���X�V����
	if (preAnimType != animType) MultiAnimator::GetPlayer(animType, moveAttribute->GetAttribute(), attackAttribute->GetAttribute(), &anim);
	
	if (0.0 < velocity.x) isReverseX = true;
	else if (velocity.x < 0.0f) isReverseX = false;

	// �`�悷��
	anim.Draw(transform.position - MultiPlayClient::offset, transform.rotation, transform.scale, Color::White, isReverseX);

	// �A�j���[�V�����^�C�v�̍X�V
	preAnimType = animType;
}


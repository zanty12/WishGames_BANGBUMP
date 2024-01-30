#include "multi_player.h"
#include "xinput.h"
#include "sprite.h"
#include "multiplay.h"
#include "multi_skillorb.h"

void ServerPlayer::Loop(void) {

	// ����������
	gravityVelocity += Vector2::Down * gravity;
	if (maxGravity >= gravityVelocity.y) gravityVelocity.y = maxGravity;

	if (moveAttribute) moveAttribute->Move();
	if (attackAttribute) attackAttribute->Attack();

	// ��������񂱃V�X�e��
	if (0.75f < Input::GetTriggerLeft(0) && 0.75f < Input::GetTriggerRight(0)) {
		ServerAttribute *tmp = attackAttribute;
		attackAttribute = moveAttribute;
		moveAttribute = tmp;
	}

	// �ړ�������
	transform.position += velocity + blownVelocity + gravityVelocity;

	// ������΂��̑��x������������
	blownVelocity *= blownFriction;
	if (blownVelocity.DistanceSq() < 1.0f) blownVelocity = Vector2::Zero;

}

void ServerPlayer::Damage(AttackServerSide *attack) {
	// �U���҂������Ȃ�I��
	if (attack->GetSelf() == this) return;

	SkillOrbDrop(attack->atkDrop);

	// �m�b�N�o�b�N��^����
	attack->KnockBack(this);
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
	ClientMovableGameObject(transform) {
	SetMoveAttribute(moveAttribute);
	SetAttackAttribute(attackAttribute);
	this->transform.scale = Vector2::One * 150;
}

void ClientPlayer::Loop(void) {
	// �������Ȃ��Ȃ����
	if (!moveAttribute || !attackAttribute) return;



	// ����ւ���ĂȂ��I�H
	if (moveAttribute->GetAttribute() == moveAttributeType && attackAttribute->GetAttribute() == attackAttributeType) {
		Update(moveAttribute, attackAttribute, &anim);
	}
	// ����������ւ���Ă�I�H
	else if (moveAttribute->GetAttribute() == attackAttributeType && attackAttribute->GetAttribute() == moveAttributeType) {
		Update(attackAttribute, moveAttribute, &reverseAnim);
	}


	// �A�j���[�V�����^�C�v�̍X�V
	preAnimType = animType;
}

void ClientPlayer::Update(ClientAttribute *moveAttribute, ClientAttribute *attackAttribute, MultiAnimator *anim) {
	// �ړ��A�j���[�V����
	if (animType == ANIMATION_TYPE_MOVE || animType == ANIMATION_TYPE_MOVE_CHARGE) {
		moveAttribute->Move();
	}
	// �U���A�j���[�V����
	if (animType == ANIMATION_TYPE_ATTACK || animType == ANIMATION_TYPE_ATTACK_CHARGE) {
		attackAttribute->Attack();
	}

	// �A�j���[�V�������؂�ւ�����u�ԁA�A�j���[�V��������ʒu���X�V����
	if (preAnimType != animType) MultiAnimator::GetPlayer(animType, moveAttribute->GetAttribute(), attackAttribute->GetAttribute(), anim);

	if (0.0 < velocity.x) isReverseX = true;
	else if (velocity.x < 0.0f) isReverseX = false;


	// �`�悷��
	anim->Draw(transform.position - MultiPlayClient::offset, transform.rotation, transform.scale, Color::White, isReverseX);
}

void ClientPlayer::SetMoveAttribute(ClientAttribute *moveAttribute) {
	// null�Ȃ珈�����Ȃ�
	if (moveAttribute == nullptr) return;

	// �폜
	if (this->moveAttribute) delete this->moveAttribute;
	// �ݒ�
	this->moveAttribute = moveAttribute;
	// �����^�C�v�̐ݒ�
	moveAttributeType = moveAttribute->GetAttribute();
	// �A�j���[�V�����̐ݒ�
	if (attackAttribute) {
		anim = MultiAnimator::GetPlayerInitialize(0, moveAttribute->GetAttribute(), attackAttribute->GetAttribute());
		reverseAnim = MultiAnimator::GetPlayerInitialize(0, attackAttribute->GetAttribute(), moveAttribute->GetAttribute());
	}
}

void ClientPlayer::SetAttackAttribute(ClientAttribute *attackAttribute) {
	// null�Ȃ珈�����Ȃ�
	if (attackAttribute == nullptr) return;

	// �폜
	if (this->attackAttribute) delete this->attackAttribute;
	// �ݒ�
	this->attackAttribute = attackAttribute;
	// �����^�C�v�̐ݒ�
	attackAttributeType = attackAttribute->GetAttribute();
	// �A�j���[�V�����̐ݒ�
	if (moveAttribute) {
		anim = MultiAnimator::GetPlayerInitialize(0, moveAttribute->GetAttribute(), attackAttribute->GetAttribute());
		reverseAnim = MultiAnimator::GetPlayerInitialize(0, attackAttribute->GetAttribute(), moveAttribute->GetAttribute());
	}
}


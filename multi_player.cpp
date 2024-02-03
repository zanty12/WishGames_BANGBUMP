#include "multi_player.h"
#include "xinput.h"
#include "sprite.h"
#include "multiplay.h"
#include "multi_skillorb.h"


void ServerPlayer::Loop(void) {
	// �_���[�W����������
	damageEffectAttributeType = -1;

	// ����������
	gravityVelocity += Vector2::Down * gravity;
	if (-maxGravity >= gravityVelocity.y) gravityVelocity.y = -maxGravity;

	// ����
	if (moveAttribute) {
		moveAttribute->LevelUpdate();
		moveAttribute->Move();
	}
	if (attackAttribute) {
		attackAttribute->LevelUpdate();
		attackAttribute->Attack();
	}

	// �����؂�ւ���������񂱁I�I
	if (0.75f < Input::GetTriggerLeft(0) && 0.75f < Input::GetTriggerRight(0)) {
		if (attributeChange == false) {
			ServerAttribute *tmp = attackAttribute;
			attackAttribute = moveAttribute;
			moveAttribute = tmp;
			attributeChange = true;
			SetPlayerAnimIdle(animType);
		}
	}
	else {
		attributeChange = false;
	}

	// ������΂��̑��x������������
	blownVelocity *= blownFriction;
	velocity *= friction;
	if (blownVelocity.DistanceSq() < 1.0f) blownVelocity = Vector2::Zero;
}

void ServerPlayer::Damage(AttackServerSide *attack) {
	// �U���҂������Ȃ�I��
	if (attack->GetSelf() == this) return;

	damageEffectAttributeType = attack->GetType();

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


	// �_���[�W�G�t�F�N�g�̃��[�h
	fireDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_fire.png"), 5, 2, 0, 7, false);
	waterDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_water.png"), 5, 2, 0, 7, false);
	thunderDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_thunder.png"), 5, 2, 0, 7, false);
	windDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_wind.png"), 5, 2, 0, 7, false);
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

	
	if (damageEffectAttributeType != -1) {
		MultiAnimator *anim = &allDamageEffect;
		if (damageEffectAttributeType == MULTI_ATTACK_FIRE) anim = &fireDamageEffect;
		else if (damageEffectAttributeType == MULTI_ATTACK_WATER) anim = &waterDamageEffect;
		else if (damageEffectAttributeType == MULTI_ATTACK_THUNDER) anim = &thunderDamageEffect;
		else if (damageEffectAttributeType == MULTI_ATTACK_WIND) anim = &windDamageEffect;

		// �_���[�W�G�t�F�N�g
		if (MultiPlayClient::GetGameMode())MultiPlayClient::GetGameMode()->GetMap()->GetEffects()->AddEffect(
			*anim,
			transform.position,
			0.0f,
			Vector2::One * transform.scale.x,
			Color::White
		);
	}
}

void ClientPlayer::ShowEntry() {
	if (entryType == ENTRY) return;

	timer.Start();
	entryType = ENTRY;

	// �A�j���[�V����
	MultiAnimator anim = MultiAnimator(LoadTexture("data/texture/Effect/effect_spawn.png"), 5, 3, 0, 9, false);
	// �������~�炷
	float height = 1000.0f;
	Vector2 localPos = Vector2(0.0f, height * 0.15f);
	MultiPlayClient::GetGameMode()->GetMap()->GetEffects()->AddEffect(anim, transform.position + localPos, 0.0f, Vector2::One * height, Color::White);
}
void ClientPlayer::ShowExit() {
	entryType = EXIT;
}

void ClientPlayer::Update(ClientAttribute *moveAttribute, ClientAttribute *attackAttribute, MultiAnimator *anim) {
	if (timer.GetNowTime() < 200ul && entryType == ENTRY || entryType == NONE) return;

	// �ҋ@�A�j���[�V����
	if (moveAttribute) {
		moveAttribute->Idle();
	}


	// �S�����A�����h�N�T�N�i�b�^
	// �ړ��A�j���[�V�����i���\���j
	if (moveAttribute) {
		if (moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_FIRE) {
			// �ړ��A�j���[�V����
			moveAttribute->Move();
		}
	}
	// �U���A�j���[�V�����i���O�\���j
	if (attackAttribute) {
		if (attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_WIND) {
			// �ړ��A�j���[�V����
			attackAttribute->Attack();
		}
	}


	// �A�j���[�V�������؂�ւ�����u�ԁA�A�j���[�V��������ʒu���X�V����
	if (preAnimType != animType) MultiAnimator::GetPlayer(animType, moveAttribute->GetAttribute(), attackAttribute->GetAttribute(), anim);

	// �v���C���[���]
	if (0.0 < velocity.x) isReverseX = true;
	else if (velocity.x < 0.0f) isReverseX = false;

	// �`�悷��
	anim->Draw(transform.position - MultiPlayClient::offset, transform.rotation, transform.scale, Color::White, isReverseX);


	// �S�����A�����h�N�T�N�i�b�^�@�p�[�g�Q
	// �ړ��A�j���[�V�����i��O�\���j
	if (moveAttribute) {
		if (moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_DARK ||
			moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_THUNDER ||
			moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_WIND) {
			// �ړ��A�j���[�V����
			moveAttribute->Move();
		}
	}
	// �U���A�j���[�V�����i��O�\���j
	if (attackAttribute) {
		if (attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_FIRE ||
			attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_DARK ||
			attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_THUNDER) {
			// �ړ��A�j���[�V����
			attackAttribute->Attack();
		}
	}








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


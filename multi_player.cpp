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

	// ������΂��̑��x������������
	blownVelocity *= blownFriction;
	velocity *= friction;

	// �����g�p�ł���Ȃ�
	if (attackAttribute->state->exNoAttributeTime <= exCoolTime.GetNowTime() * 0.001f) {
		// ����
		if (moveAttribute) {
			moveAttribute->LevelUpdate();
			moveAttribute->Move();
			moveAttribute->MpUpdate();
		}
		if (attackAttribute) {
			attackAttribute->LevelUpdate();
			attackAttribute->Attack();
			attackAttribute->MpUpdate();
		}
	}

	// �����؂�ւ���������񂱁I�I
	if (attackAttribute->state->exCoolTime <= exCoolTime.GetNowTime() * 0.001f) {
		if (0.75f < Input::GetTriggerLeft(0) && 0.75f < Input::GetTriggerRight(0)) {
			ServerAttribute *tmp = attackAttribute;
			attackAttribute = moveAttribute;
			moveAttribute = tmp;
			SetPlayerAnimIdle(animType);

			exCoolTime.Start();
		}
	}
	if (blownVelocity.DistanceSq() < 1.0f) blownVelocity = Vector2::Zero;
}

void ServerPlayer::Damage(AttackServerSide *attack) {
	// �U���҂������Ȃ�I��
	if (attack->GetSelf() == this) return;

	// �_���[�W�G�t�F�N�g
	damageEffectAttributeType = attack->GetType();

	// �X�L���I�[�u�̃h���b�v
	if (MultiPlayServer::GetGameMode()->GetMode() == MULTI_MODE::FINAL_BATTLE ||
		attack->GetType() == MULTI_OBJECT_TYPE::MULTI_ATTACK_ENEMY2 ||
		attack->GetType() == MULTI_OBJECT_TYPE::MULTI_SPIKE) {
		SkillOrbDrop(attack->atkDrop);
	}

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
	// ���x���͈͂̓ǂݍ���
	std::wstring lvStr = L"lv";
	for (int i = 0; i < MAX_LV; i++) {
		lvupPoint[i] = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", lvStr + std::to_wstring(i + 1), 0);
	}


	SetMoveAttribute(moveAttribute);
	SetAttackAttribute(attackAttribute);
	this->transform.scale = Vector2::One * 150;

	preMoveAttributeType = moveAttributeType;
	preAttackAttributeType = attackAttributeType;

	// �_���[�W�G�t�F�N�g�̃��[�h
	allDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_all.png"), 5, 2, 0, 7, false);
	fireDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_fire.png"), 5, 2, 0, 7, false);
	waterDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_water.png"), 5, 2, 0, 7, false);
	thunderDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_thunder.png"), 5, 2, 0, 7, false);
	windDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_wind.png"), 5, 2, 0, 7, false);
	// ���̑��G�t�F�N�g
	exEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_EX.png"), 5, 6, 0, 29, false);
	lvUpEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_levelup.png"), 5, 6, 0, 29, false);
	lvDownEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_leveldown.png"), 5, 6, 0, 29, false);

	lvUpUI = MultiAnimator(LoadTexture("data/texture/UI/UI_levelup.png"), 5, 6, 0, 29, false);
	lvDownUI = MultiAnimator(LoadTexture("data/texture/UI/UI_leveldown.png"), 5, 6, 0, 29, false);

	exEffect.SetFrame(1000 / 42);
	exEffect.MoveEnd();
	lvUpEffect.MoveEnd();
	lvDownEffect.MoveEnd();
	lvUpUI.MoveEnd();
	lvDownUI.MoveEnd();
}

void ClientPlayer::Loop(void) {
	// �������Ȃ��Ȃ����
	if (!moveAttribute || !attackAttribute) return;

	// ��]���Z�b�g
	if (!isRotationAttributeControl) transform.rotation = 0.0f;



	// ��������񂱃A�j���[�V�����J�n
	if (preMoveAttributeType == attackAttributeType &&
		preAttackAttributeType == moveAttributeType) {
		exEffect.MoveBegin();
	}

	// �A�j���[�V�������J�n����Ă��Ȃ��Ȃ�A���ݔ������Ă��鑮�������ꂼ��w�肷��
	if (18 < exEffect.GetIndex()) {
		// ����ւ���ĂȂ��I�H
		if (moveAttribute->GetAttribute() == moveAttributeType && attackAttribute->GetAttribute() == attackAttributeType) {
			curMoveAttribute = moveAttribute, curAttackAttribute = attackAttribute;
			curAnim = &anim;
		}
		// ����������ւ���Ă�I�H
		else if (moveAttribute->GetAttribute() == attackAttributeType && attackAttribute->GetAttribute() == moveAttributeType) {
			curMoveAttribute = attackAttribute, curAttackAttribute = moveAttribute;
			curAnim = &reverseAnim;
		}
	}

	if (curAnim) Update(curMoveAttribute, curAttackAttribute, curAnim);



	// �A�j���[�V�����^�C�v�̍X�V
	preAnimType = animType;

	exEffect.Draw(transform.position - MultiPlayClient::offset, 0.0f, transform.scale * 1.8f, Color::White);
	preMoveAttributeType = moveAttributeType;
	preAttackAttributeType = attackAttributeType;

	if (damageEffectAttributeType != -1) {
		MultiAnimator *anim = &allDamageEffect;
		if (damageEffectAttributeType == MULTI_ATTACK_FIRE) anim = &fireDamageEffect;
		else if (damageEffectAttributeType == MULTI_ATTACK_WATER) anim = &waterDamageEffect;
		else if (damageEffectAttributeType == MULTI_ATTACK_THUNDER) anim = &thunderDamageEffect;
		else if (damageEffectAttributeType == MULTI_ATTACK_WIND) anim = &windDamageEffect;
		else if (damageEffectAttributeType == MULTI_ATTACK_ENEMY2) anim = &allDamageEffect;

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
void ClientPlayer::DrawUI(void) {
	if (curAttackAttribute) curAttackAttribute->DrawUI();
}

void ClientPlayer::Update(ClientAttribute *moveAttribute, ClientAttribute *attackAttribute, MultiAnimator *anim) {
	if (timer.GetNowTime() < 200ul && entryType == ENTRY || entryType == NONE) return;

	// ���x�����擾
	lv = GetLv();

	// �v���C���[���]
	if (!isReverseXAttributeControl) {
		if (0.0 < velocity.x) isReverseX = true;
		else if (velocity.x < 0.0f) isReverseX = false;
	}

	// �ҋ@�A�j���[�V����
	if (moveAttribute) {
		moveAttribute->Idle();
	}


	// �S�����A�����h�N�T�N�i�b�^
	// �ړ��A�j���[�V�����i���\���j
	if (moveAttribute) {
		if (moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_FIRE) {
			// �ړ��A�j���[�V����
			moveAttribute->LevelUpdate();
			moveAttribute->Move();
		}
	}
	// �U���A�j���[�V�����i���O�\���j
	if (attackAttribute) {
		if (attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_WIND) {
			// �ړ��A�j���[�V����
			attackAttribute->LevelUpdate();
			attackAttribute->Attack();
		}
	}


	// �A�j���[�V�������؂�ւ�����u�ԁA�A�j���[�V��������ʒu���X�V����
	if (preAnimType != animType) MultiAnimator::GetPlayer(animType, moveAttribute->GetAttribute(), attackAttribute->GetAttribute(), anim);

	// �`�悷��
	anim->Draw(transform.position - MultiPlayClient::offset, transform.rotation, transform.scale, Color::White, isReverseX);


	// �S�����A�����h�N�T�N�i�b�^�@�p�[�g�Q
	// �ړ��A�j���[�V�����i��O�\���j
	if (moveAttribute) {
		if (moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_DARK ||
			moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_THUNDER ||
			moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_WIND) {
			// �ړ��A�j���[�V����
			moveAttribute->LevelUpdate();
			moveAttribute->Move();
		}
	}
	// �U���A�j���[�V�����i��O�\���j
	if (attackAttribute) {
		if (attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_FIRE ||
			attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_DARK ||
			attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_THUNDER) {
			// �ړ��A�j���[�V����
			attackAttribute->LevelUpdate();
			attackAttribute->Attack();
		}
	}




	if (preLv < lv) {
		lvUpEffect.MoveBegin();
		lvUpUI.MoveBegin();
	}
	else if (lv < preLv) {
		lvDownEffect.MoveBegin();
		lvDownUI.MoveBegin();
	}
	lvUpEffect.Draw(transform.position - MultiPlayClient::offset, 0.0f, transform.scale * 1.5f, Color::White);
	lvDownEffect.Draw(transform.position - MultiPlayClient::offset, 0.0f, transform.scale * 1.5f, Color::White);
	lvUpUI.Draw(transform.position - MultiPlayClient::offset, 0.0f, transform.scale, Color::White);
	lvDownUI.Draw(transform.position - MultiPlayClient::offset, 0.0f, transform.scale, Color::White);
	preLv = lv;
}

void ClientPlayer::SetMoveAttribute(ClientAttribute *moveAttribute) {
	// null�Ȃ珈�����Ȃ�
	if (moveAttribute == nullptr) return;

	// �폜
	if (this->moveAttribute) delete this->moveAttribute;
	// �ݒ�
	this->moveAttribute = moveAttribute;
	// �����^�C�v�̐ݒ�
	moveAttributeType = preMoveAttributeType = moveAttribute->GetAttribute();
	// �A�j���[�V�����̐ݒ�
	if (attackAttribute) {
		anim = MultiAnimator::GetPlayerInitialize(id % 4, moveAttribute->GetAttribute(), attackAttribute->GetAttribute());
		reverseAnim = MultiAnimator::GetPlayerInitialize(id % 4, attackAttribute->GetAttribute(), moveAttribute->GetAttribute());
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
	attackAttributeType = preAttackAttributeType = attackAttribute->GetAttribute();
	// �A�j���[�V�����̐ݒ�
	if (moveAttribute) {
		anim = MultiAnimator::GetPlayerInitialize(id % 4, moveAttribute->GetAttribute(), attackAttribute->GetAttribute());
		reverseAnim = MultiAnimator::GetPlayerInitialize(id % 4, attackAttribute->GetAttribute(), moveAttribute->GetAttribute());
	}
}


#include "lib/math.h"
#include "multi_attribute.h"
#include "multiplay.h"
#include "lib/collider2d.h"
#include "xinput.h"
#include "time.h"
#include "multi_ui.h"


AttackServerSide *ServerAttribute::CreateAttack(void) {
	return nullptr;
};
bool ServerAttribute::DestroyAttack(void) {
	// �U���폜
	if (attack_ && state->atkAfterTime <= atkAfterTimer.GetNowTime() * 0.001f) {
		attack_->Destroy();
		attack_ = nullptr;
		return true;
	}

	return false;
};
void ServerAttribute::LevelUpdate(void) {
	int lv = MAX_LV - 1;
	// ���x��������̏ꍇ
	if (player->lvupPoint[lv] <= player->skillPoint) {
		state = &state_lv[lv];
		this->lv = lv;
	}

	// ���x���̒���
	for (lv = 0; lv < MAX_LV - 1; lv++) {
		if (player->lvupPoint[lv] <= player->skillPoint && player->skillPoint < player->lvupPoint[lv + 1]) {
			state = &state_lv[lv];
			this->lv = lv;
			return;
		}
	}
}
void ServerAttribute::MpUpdate(void) {
	if (state->healSpanTime <= skillMpTimer.GetNowTime() * 0.001f) {
		mp += state->heal;
		skillMpTimer.Start();
	}

	if (state->maxMp < mp) mp = state->maxMp;
	else if (mp < state->minMp) mp = state->minMp;
}
bool ServerAttribute::IsUseMp(void) {
	if (state->cost <= mp) {
		mp -= state->cost;
		return true;
	}
	return false;
}

ServerAttribute *ServerAttribute::Create(ServerPlayer *player, ATTRIBUTE_TYPE type) {
	switch (type) {
	case ATTRIBUTE_TYPE_FIRE: return new ServerFire(player);
	case ATTRIBUTE_TYPE_DARK: return new ServerWater(player);
	case ATTRIBUTE_TYPE_THUNDER: return new ServerThunder(player);
	case ATTRIBUTE_TYPE_WIND: return new ServerWind(player);
	}
	return nullptr;
}
void ServerAttribute::AddPower(void) {
	power += state->addPower;
	if (state->maxPower < power) power = state->maxPower;
	else if (power < state->minPower) power = state->minPower;
}
void ServerAttribute::AddPower(float scaler) {
	power += scaler * state->inputPowerRate;
	if (state->maxPower < power) power = state->maxPower;
	else if (power < state->minPower) power = state->minPower;
}
void ServerAttribute::AddPower(Vector2 vector) {
	power += state->addPower + vector.Distance() * state->inputPowerRate;
	if (state->maxPower < power) power = state->maxPower;
	else if (power < state->minPower) power = state->minPower;
}
void ServerAttribute::FrictionPower(void) {
	power *= state->powerFriction;
}
void ServerAttribute::Friction(void) {
	player->velocity *= state->friction;
}
Vector2 ServerAttribute::CalcVector(Vector2 stick) {
	return stick * power;
}
ClientAttribute *ClientAttribute::Create(ClientPlayer*player, ATTRIBUTE_TYPE type) {
	switch (type) {
	case ATTRIBUTE_TYPE_FIRE: return new ClientFire(player);
	case ATTRIBUTE_TYPE_DARK: return new ClientWater(player);
	case ATTRIBUTE_TYPE_THUNDER: return new ClientThunder(player);
	case ATTRIBUTE_TYPE_WIND: return new ClientWind(player);
	}
	return nullptr;
}
void ClientAttribute::LevelUpdate(void) {
	int lv = MAX_LV - 1;
	// ���x��������̏ꍇ
	if (player->lvupPoint[lv] <= player->skillPoint) {
		state = &state_lv[lv];
	}

	// ���x���̒���
	for (lv = 0; lv < MAX_LV - 1; lv++) {
		if (player->lvupPoint[lv] <= player->skillPoint && player->skillPoint < player->lvupPoint[lv + 1]) {
			state = &state_lv[lv];
			return;
		}
	}
}










/*******************************************************
  Fire
********************************************************/
bool ServerFire::StickTrigger(Vector2 stick, Vector2 previousStick) {
	return stick.DistanceSq() > state->minInputDistance * state->minInputDistance;
}

void ServerFire::Move(void) {
	Vector2 stick = Input::GetStickLeft(0);


	// �ړ���
	if (StickTrigger(stick)) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimMove(player->animType);

		// �p���[���Z
		AddPower(stick);

		// �x�N�g���v�Z
		velocity += CalcVector(stick) * state->powerMoveRatio;

		// �d�͂��Ȃ���
		player->gravityVelocity = Vector2::Zero;

		// �x�N�g���̍ő�l
		float maxPowerSq = state->maxPower * state->maxPower;
		if (maxPowerSq < velocity.DistanceSq()) velocity = velocity.Normalize() * state->maxPower;

	}
	// ��~��
	else {
		// �A�j���[�V�����̎w��
		SetPlayerAnimNoMove(player->animType);
	}

	velocity *= state->friction;
	player->velocity = velocity;


	// ���C
	Friction();
}
void ServerFire::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);


	// �U����
	if (StickTrigger(stick)) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimAttack(player->animType);

		player->attackVelocity = stick;

		// �U���I�u�W�F�N�g�̐���
		CreateAttack();

		// �A�^�b�N�ړ�
		if (attack_ && IsUseMp()) {
			attack_->transform.position = player->transform.position;
			attack_->direction = stick.Normalize() * state->atkDistance;
			attack_->transform.rotation = std::atan2(stick.y, stick.x);
		}

	}
	else if (attack_ != nullptr) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimNoAttack(player->animType);

		// �U���I�u�W�F�N�g�̍폜
		DestroyAttack();
	}
}
AttackServerSide *ServerFire::CreateAttack(void) {
	// �U���I�u�W�F�N�g����������Ă���Ȃ�I��
	if (attack_) return nullptr;
	// �N�[���^�C�����Ȃ�I��
	if (state->coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < state->coolTime) return nullptr;



	// �U������
	attack_ = player->map->GetAttacks()->Add<ServerFireAttack>(player, this);

	// �N�[���^�C���v���J�n
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientFire::Move(void) {
	// �ړ��A�j���[�V�����ł͂Ȃ��Ȃ�I��
	if (!IsPlayerAnimMove(player->animType)) {
		if (moveAnims.size()) moveAnims.clear();
		return;
	}



	// �`�悷��
	Vector2 direction = player->velocity;
	float denominator = moveAnims.size();
	float numerator = denominator * 0.5f;
	for (Animator &anim : moveAnims) {
		anim.anim.Draw(anim.pos - MultiPlayClient::offset, anim.rot, anim.scl, Color(1.0f, 1.0f, 1.0f, 1.0f/* - numerator / denominator*/));
		numerator += 0.5f;
	}

	// ���Ԃ��v������
	DWORD currentTime = timeGetTime();
	DWORD deltaTime = currentTime - startTime;

	// �A�j���[�V�������쐬����
	if (25 < deltaTime) {
		// �v�������Z�b�g����
		startTime = currentTime;

		// ���̈ʒu�����炷
		direction += direction.Normal() * MATH::Rand(-0.25f, 0.25f);

		// �A�j���[�V��������
		Vector2 pos = player->transform.position + -direction.Normalize();
		float rot = atan2f(direction.y, -direction.x);
		Vector2 scl = Vector2(state->showMoveX, state->showMoveY);
		Color col = Color::White;
		moveAnims.push_front({ pos, rot, scl, moveAnim });

		// �v�f�������Ȃ�폜
		if (2 < moveAnims.size()) {
			moveAnims.pop_back();
		}
	}
}
void ClientFire::Attack(void) {
	// �U���A�j���[�V�����ł͂Ȃ��Ȃ�I��
	if (!IsPlayerAnimAttack(player->animType)) {
		player->isReverseXAttributeControl = false;
		return;
	}


	// ���x���ɂ���ăe�N�X�`���̕ύX
	if (player->GetLv() < 6) {
		attackAnim.texNo = attackTexNo;
		attackAnim.width = 5;
		attackAnim.height = 6;
		attackAnim.end = 25;
	}
	else {
		attackAnim.texNo = attack2TexNo;
		attackAnim.width = 5;
		attackAnim.height = 6;
		attackAnim.end = 29;
	}

	// �U���̌���
	Vector2 direction = player->attackVelocity.Normalize() * state->showAttackY * 0.5f;

	// �������ŃR���g���[�����邩���߂�
	player->isReverseXAttributeControl = direction != Vector2::Zero;

	// �L�����𔽓]������
	if (player->isReverseXAttributeControl) {
		if (direction.x < 0.0f) player->isReverseX = false;
		else if (direction.x > 0.0f) player->isReverseX = true;
	}

	// ��Ɉړ�
	Vector2 localPos = Vector2(-2.5f, 40.0f);

	// �g�����X�t�H�[�}�[
	Vector2 pos = player->transform.position;
	float rot = atan2f(direction.x, direction.y);
	Vector2 scl = Vector2(state->showAttackX, state->showAttackY);

	// ���]�����G�ɍ��킹�Ĉʒu�����]
	if (player->isReverseX) localPos.x *= -1.0f;

	// �`�悷��
	attackAnim.Draw(pos + direction + localPos - MultiPlayClient::offset, rot, scl, Color::White);
}
void ClientFire::DrawUI(void) {
	Vector2 localPos = Vector2(0, player->transform.scale.y * 0.5f + 3);
	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	Vector2 scl = Vector2(35, 15);

	float ratio = (float)mp / (float)state->maxMp;
	DrawSpriteBoxEffectLeftToRight(frameUITexNo, pos + localPos, scl, Color::White, 1.0f);
	DrawSpriteBoxEffectLeftToRight(uiTexNo, pos + localPos, scl, Color::White, ratio);
}

void ServerFireAttack::Loop(void) {

}
void ServerFireAttack::KnockBack(ServerMovableGameObject *object) {
	object->blownVelocity = self->Cast<ServerPlayer>()->attackVelocity * knockbackRate;
}





/*******************************************************
  Water
********************************************************/
bool ServerWater::StickTrigger(Vector2 stick, Vector2 previousStick) {


	return false;
}
void ServerWater::Move(void) {
	Vector2 stick = Input::GetStickLeft(0);


	// ���[�v
	if (Input::GetKeyDown(0, Input::LThumb)) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimMove(player->animType);

		// �ړ�
		player->transform.position = player->chargeVelocity;

		// ������
		power = 0.0f;
	}
	// ���[�v�����̃`���[�W
	else if (stick != Vector2::Zero) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimMove(player->animType, true);

		// �p���[���Z
		AddPower(stick);

		// ���[�v�x�N�g���̎w��
		player->chargeVelocity = player->transform.position + CalcVector(stick) * state->powerMoveRatio;

		// �d�͂����X�ɂȂ���
		player->gravityVelocity *= state->friction;
	}
	else {
		// �A�j���[�V�����̎w��
		SetPlayerAnimNoMove(player->animType);

		power = 0.0f;
	}

	player->velocity *= state->friction;
}
void ServerWater::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);
	bool isCharge = IsPlayerAnimAttackCharge(player->animType);
	bool isAttack = IsPlayerAnimAttack(player->animType);

	// �U�����ĂȂ�
	if (!isAttack && !isCharge) {
		// �U���J�n
		if (Input::GetKey(0, Input::RThumb) && IsUseMp()) {
			// �A�j���[�V�����̎w��
			SetPlayerAnimAttack(player->animType, true);

			// ���Ԍv���J�n
			attackTimer.Start();
		}
		// �`���[�W
		else if (stick != Vector2::Zero) {
			// ���[�v�x�N�g���̎w��
			player->attackVelocity = stick.Normalize();

			// �p���[���Z
			AddPower(stick);
		}
		else {
			// �A�j���[�V�����̎w��
			SetPlayerAnimNoAttack(player->animType);

			// ������
			power = 0.0f;
		}
	}

	// ���ߍU��
	if (isCharge) {
		// ���ߍU���i0.5�b�j
		if (0.5f <= attackTimer.GetNowTime() * 0.001f) {

			// �U���I�u�W�F�N�g�̐���
			CreateAttack();
		}
	}

	// �U����
	if (attack_) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimAttack(player->animType);

		// �A�^�b�N�ړ�
		attack_->transform.position = player->transform.position;
		attack_->direction = player->attackVelocity * state->atkDistance;
	}

	if (isAttack || isCharge) {
		// ���C��R
		player->velocity = Vector2::Zero;
	}

	// �U���I�u�W�F�N�g���폜�\�Ȃ�
	if (DestroyAttack()) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimNoAttack(player->animType);
	}
}
AttackServerSide *ServerWater::CreateAttack(void) {
	// �U���I�u�W�F�N�g����������Ă���Ȃ�I��
	if (attack_) return nullptr;
	// �N�[���^�C�����Ȃ�I��
	if (state->coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < state->coolTime) return nullptr;



	// �U������
	attack_ = player->map->GetAttacks()->Add<ServerWaterAttack>(player, this);

	// �N�[���^�C���v���J�n
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientWater::Move(void) {
	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	float rot = 0.0f;
	Vector2 scl = Vector2(state->showMoveX, state->showMoveY);
	Color col = Color::White;

	// �ړ�
	if (IsPlayerAnimMove(player->animType)) {
		moveAnim.MoveBegin();
	}
	// �`���[�W
	else if (IsPlayerAnimMoveCharge(player->animType)) {
		moveChargeAnim.Draw(pos, rot, scl, col);

		if (player->id == MultiPlayClient::GetID()) {
			// �ړ���̕`��
			indicator.Draw(
				player->chargeVelocity - MultiPlayClient::offset,
				player->transform.rotation,
				player->transform.scale,
				Color(1.0f, 1.0f, 1.0f, 0.5f)
			);
		}

		// ���ݒn���L�^
		if(moveAnim.IsEnd()) prevPosition = player->transform.position;
	}

	moveAnim.Draw(player->transform.position - MultiPlayClient::offset + Vector2(0.0f, state->showMoveY * 0.5f), rot, scl * 2.0f, col);
}
void ClientWater::Attack(void) {
	// �C���W�P�[�^�[
	if (player->id == MultiPlayClient::GetID()) {
		Vector2 stick = Input::GetStickRight(0);
		if (stick != Vector2::Zero) {
			Vector2 localPos = stick.Normalize() * 100.0f;
			Vector2 pos = player->transform.position - MultiPlayClient::offset;
			float rot = atan2f(-localPos.y, localPos.x);
			Vector2 scl = Vector2::One * 100.0f;
			Color col = Color::White;

			attackIndicator.Draw(pos + localPos, rot, scl, col);
		}
	}





	// �U���̌���
	Vector2 direction = player->attackVelocity.Normalize() * state->showAttackY * 0.5f;

	// ���x���ɂ���ăe�N�X�`���̕ύX
	if (player->GetLv() < 6) {
		attackAnim.texNo = attackTexNo;
		attackAnim.width = 5;
		attackAnim.height = 6;
		attackAnim.end = 29;
	}
	else {
		attackAnim.texNo = attack2TexNo;
		attackAnim.width = 5;
		attackAnim.height = 6;
		attackAnim.end = 29;
	}

	// ��Ɉړ�
	Vector2 localPos = Vector2(-25.0f, 25.0f);

	// �g�����X�t�H�[�}�[
	Vector2 pos = player->transform.position;
	float rot = atan2f(direction.x, direction.y);
	Vector2 scl = Vector2(state->showAttackX, state->showAttackY);







	// �U��
	if (IsPlayerAnimAttack(player->animType)) {

		// ��]
		player->transform.rotation = rot;

		// �L�����𔽓]������
		player->isReverseXAttributeControl = true;
		player->isRotationAttributeControl = true;
		if (direction.x < 0.0f) {
			player->isReverseX = false;
			player->transform.rotation += MATH::Deg2Rad * 90.0f;
		}
		else if (direction.x > 0.0f) {
			player->isReverseX = true;
			player->transform.rotation -= MATH::Deg2Rad * 90.0f;
		}



		// ���]�����G�ɍ��킹�Ĉʒu�����]
		if (player->isReverseX) {
			float tmpRot = MATH::PI * 0.5f - rot;
			float rotX = std::cos(tmpRot) * -localPos.x - std::sin(tmpRot) * localPos.y;
			float rotY = std::sin(tmpRot) * -localPos.x + std::cos(tmpRot) * localPos.y;
			localPos.x = rotX;
			localPos.y = rotY;
		}
		else {
			float tmpRot = MATH::PI + MATH::PI * 0.5f - rot;
			float rotX = std::cos(tmpRot) * localPos.x - std::sin(tmpRot) * localPos.y;
			float rotY = std::sin(tmpRot) * localPos.x + std::cos(tmpRot) * localPos.y;
			localPos.x = rotX;
			localPos.y = rotY;
		}

		// �`�悷��
		attackAnim.Draw(pos + direction + localPos - MultiPlayClient::offset, rot, scl, Color::White);
		attackSplashAnim.Draw(
			pos + direction.Normalize() * state->showAttackX * 0.08 + localPos - MultiPlayClient::offset,
			rot,
			Vector2::One * state->showAttackX * 0.5f,
			Color::White
		);



		attackChargeAnim.MoveBegin();
	}
	// ���ߍU��
	else if (IsPlayerAnimAttackCharge(player->animType)) {

		// ��]
		player->transform.rotation = rot;

		// �L�����𔽓]������
		player->isReverseXAttributeControl = true;
		player->isRotationAttributeControl = true;
		if (direction.x < 0.0f) {
			player->isReverseX = false;
			player->transform.rotation += MATH::Deg2Rad * 90.0f;
		}
		else if (direction.x > 0.0f) {
			player->isReverseX = true;
			player->transform.rotation -= MATH::Deg2Rad * 90.0f;
		}


		// ���]�����G�ɍ��킹�Ĉʒu�����]
		if (player->isReverseX) {
			float tmpRot = MATH::PI * 0.5f - rot;
			float rotX = std::cos(tmpRot) * -localPos.x - std::sin(tmpRot) * localPos.y;
			float rotY = std::sin(tmpRot) * -localPos.x + std::cos(tmpRot) * localPos.y;
			localPos.x = -rotX;
			localPos.y = rotY;
		}
		else {
			float tmpRot = MATH::PI + MATH::PI * 0.5f - rot;
			float rotX = std::cos(tmpRot) * localPos.x - std::sin(tmpRot) * localPos.y;
			float rotY = std::sin(tmpRot) * localPos.x + std::cos(tmpRot) * localPos.y;
			localPos.x = rotX;
			localPos.y = rotY;
		}



		// �`�悷��
		rot = atan2f(-direction.y, direction.x);

		// �L�����𔽓]������
		player->isReverseXAttributeControl = true;
		if (direction.x < 0.0f) player->isReverseX = false;
		else if (direction.x > 0.0f) player->isReverseX = true;

		// ���]�����G�ɍ��킹�Ĉʒu�����]
		if (player->isReverseX) localPos.x *= -1.0f;

		attackChargeAnim.Draw(pos + localPos - MultiPlayClient::offset, rot, Vector2::One * 100.0f, Color::White);
	}
	// ���̑�
	else {
		player->isReverseXAttributeControl = false;
		player->isRotationAttributeControl = false;
	}
}
void ClientWater::Idle(void) {
	float localScale = 75.0f;


	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	float rot = 0.0f;
	Vector2 scl = Vector2::One * localScale;
	Color col = Color(1.0f,1.0f,1.0f, 0.5f);

	// �ҋ@
	if (player->animType == ANIMATION_TYPE_IDLE) {
		idle.Draw(pos + Vector2(0.0f, -73.0f), rot, scl, col);
	}
}
void ClientWater::DrawUI(void) {
	Vector2 localPos = Vector2(player->transform.scale.y * 0.15f, player->transform.scale.y * 0.5f);
	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	Vector2 scl = Vector2::One * 20.0f;
	Color col = Color::White;

	float ratio = (float)mp / (float)state->maxMp;
	if (mp < state->cost) col *= 0.25f;
	DrawSpriteBoxEffectBottomToUp(frameUITexNo, pos + localPos, scl, Color::White, 1.0f);
	DrawSpriteBoxEffectBottomToUp(uiTexNo, pos + localPos, scl, col, ratio);
}

void ServerWaterAttack::Loop(void) {
}
void ServerWaterAttack::KnockBack(ServerMovableGameObject *object) {
	object->blownVelocity = self->Cast<ServerPlayer>()->attackVelocity * knockbackRate;
}





/*******************************************************
  Thunder
********************************************************/
bool ServerThunder::StickTrigger(Vector2 stick, Vector2 previousStick) {
	float distance = stick.Distance();
	float previousDistance = previousStick.Distance();
	float speed = (stick - previousStick).Distance();

	//Charge
	if (state->minInputDistance <= distance) {
		// �p���[���Z
		AddPower(stick * speed);
	}

	//Release
	if (distance < state->minInputDistance && state->minInputDistance < previousDistance &&
		state->minInputSpeed < MATH::Abs(distance - previousDistance)) {
		return true;
	}
	return false;
}
void ServerThunder::Move(void) {
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);

	// �`���[�W��
	if (stick != Vector2::Zero && 0.0f < power) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimMove(player->animType, true);

		player->chargeVelocity = -stick;
	}
	// �ړ����I�������i�ړ��I���j
	else if (IsPlayerAnimMove(player->animType) && player->velocity.DistanceSq() < 25.0f) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimNoMove(player->animType);

		// ���C��R�̏�����
		friction = state->friction;
	}
	// �ړ���
	else if (IsPlayerAnimMove(player->animType)) {
		// ���C��R�̉��Z
		friction -= 0.001f;
		if (friction < 0.0f) friction = 0.0f;
	}
	// �������Ă��Ȃ��̂Ƀ`���[�W�G�t�F�N�g�ɂȂ錻�ۂ������i�������X�e�B�b�N��߂����ƂōU���g���K�[����������Ȃ��j
	if (IsPlayerAnimMoveCharge(player->animType) && stick == Vector2::Zero) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimNoMove(player->animType);
	}

	// �ړ�
	if (StickTrigger(stick, previousStick)) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimMove(player->animType);

		Vector2 direction = (stick - previousStick).Normalize();
		player->velocity = CalcVector(direction) * state->powerMoveRatio;

		// ������
		power = 0.0f;

		// ���C��R�̏�����
		friction = state->friction;
	}
	
	player->velocity *= friction;
}
void ServerThunder::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);
	Vector2 previousStick = Input::GetPreviousStickRight(0);

	// �`���[�W��
	if (stick != Vector2::Zero && 0.0f < power) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimAttack(player->animType, true);

		player->chargeVelocity = -stick;
	}
	// �U�����I�������i�U���I���j
	else if (IsPlayerAnimAttack(player->animType)) {
		if (300 < chargeToAttackTimer.GetNowTime()) {
			// �A�j���[�V�����̎w��
			SetPlayerAnimNoAttack(player->animType);
		}
	}
	// �������Ă��Ȃ��̂Ƀ`���[�W�G�t�F�N�g�ɂȂ錻�ۂ������i�������X�e�B�b�N��߂����ƂōU���g���K�[����������Ȃ��j
	if (IsPlayerAnimAttackCharge(player->animType) && stick==Vector2::Zero) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimNoAttack(player->animType);
	}

	// �U��
	if (StickTrigger(stick, previousStick)) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimAttack(player->animType);

		// ���Ԍv��
		chargeToAttackTimer.Start();

		Vector2 direction = (stick - previousStick).Normalize();


		// �p���[������Ȃ�
		if (state->minPower <= power && 0.01f < direction.DistanceSq()) {
			if (IsUseMp()) {

				// �U���I�u�W�F�N�g�̐���
				auto attack = CreateAttack();

				// �A�^�b�N�ړ�
				if (attack) {
					Vector2 localPos = direction.Normal().Normalize() * 40.0f;
					if (0.0f > localPos.y) localPos *= -1.0f;

					attack->transform.position = player->transform.position + localPos;
					//attack->direction = direction.Normalize() * state->atkDistance;
					attack->velocity = CalcVector(direction) * state->powerAttackRatio;
					attack->radius = state->atkRange * 1.5f;
					attack->atk = state->atk;
					attack->atkDrop = state->atkDrop;
					attack->knockbackRate = state->knockbackRate;
					attack->localPosition = direction.Normalize() * -state->atkDistance * 0.5f;
					// �U���x�N�g���̎w��
					player->attackVelocity = direction.Normalize();

				}
			}
		}

		// ������
		power = 0.0f;
	}

	Friction();
}
AttackServerSide *ServerThunder::CreateAttack(void) {
	// �N�[���^�C�����Ȃ�I��
	if (state->coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < state->coolTime) return nullptr;



	// �U������
	// ���x���ɂ���ĕύX
	if (player->GetLv() < 6) attack_ = player->map->GetAttacks()->Add<ServerThunderAttack>(player, this);
	else attack_ = player->map->GetAttacks()->Add<ServerThunder2Attack>(player, this);
	

	// �N�[���^�C���v���J�n
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientThunder::Move(void) {
	// �C���W�P�[�^�[
	if (player->id == MultiPlayClient::GetID()) {
		Vector2 stick = Input::GetStickLeft(0);
		if (stick != Vector2::Zero) {
			Vector2 localPos = -stick.Normalize() * 100.0f;
			Vector2 pos = player->transform.position - MultiPlayClient::offset;
			float rot = atan2f(-localPos.y, localPos.x);
			Vector2 scl = Vector2::One * 100.0f;
			Color col = Color::White;

			indicator.Draw(pos + localPos, rot, scl, col);
		}
	}





	if (!IsPlayerAnimMove(player->animType) && !IsPlayerAnimMoveCharge(player->animType)) {
		player->isRotationAttributeControl = false;
		return;
	}
	player->isRotationAttributeControl = true;

	// �ړ��J�n
	if (!IsPlayerAnimMove(player->preAnimType) && IsPlayerAnimMove(player->animType)) {
		float distance = player->velocity.Distance();
		Vector2 pos = player->transform.position + player->velocity * 0.5f;
		float rot = std::atan2(player->velocity.y, -player->velocity.x);
		Vector2 scl = Vector2(distance * 7.0f, player->transform.scale.x * 0.75f);

		// ��]
		player->isRotationAttributeControl = true;
		player->transform.rotation = rot;

		// �L�����𔽓]������
		if (player->velocity.x < 0.0f) {
		}
		else if (player->velocity.x > 0.0f) {
			player->transform.rotation += MATH::Deg2Rad * 180.0f;
		}

		//MultiPlayClient::GetGameMode()->GetMap()->GetEffects()->AddEffect(moveAnim, pos, rot, scl);
	}

	if (IsPlayerAnimMoveCharge(player->preAnimType)) {
		// �`���[�W�`��
		chargeAttackAnim.Draw(player->transform.position - MultiPlayClient::offset, 0.0f, player->transform.scale * 1.2f, Color::White);
	}
}
void ClientThunder::Attack(void) {
	if (!IsPlayerAnimAttack(player->animType) && !IsPlayerAnimAttackCharge(player->animType)) {
		player->isReverseXAttributeControl = false;
		player->isRotationAttributeControl = false;
		return;
	}



	Vector2 direction = player->chargeVelocity;

	// �������ŃR���g���[�����邩���߂�
	player->isRotationAttributeControl = player->isReverseXAttributeControl = direction != Vector2::Zero;

	// �L�����𔽓]������
	if (player->isReverseXAttributeControl) {
		float rot = std::atan2(direction.x, direction.y);

		// ��]
		player->transform.rotation = rot;

		// �L�����𔽓]������
		player->isRotationAttributeControl = true;
		if (direction.x < 0.0f) {
			player->isReverseX = false;
			player->transform.rotation += MATH::Deg2Rad * 90.0f;
		}
		else if (direction.x > 0.0f) {
			player->isReverseX = true;
			player->transform.rotation -= MATH::Deg2Rad * 90.0f;
		}


		if (direction.x < 0.0f) player->isReverseX = false;
		else if (direction.x > 0.0f) player->isReverseX = true;
	}

	// �`���[�W��
	if (IsPlayerAnimAttackCharge(player->animType)) {
		Vector2 localPos = direction.Normal().Normalize() * 40.0f;
		Vector2 pos = player->transform.position - MultiPlayClient::offset;
		float rot = atan2f(-direction.y, direction.x);
		Vector2 scl = Vector2::One * 150.0f;
		Color col = Color::White;

		if (player->isReverseX) localPos *= -1.0f;

		// ���x���ɂ���ăe�N�X�`���̕ύX
		if (player->GetLv() < 6) {
			attackAnim.texNo = attackTexNo;
			attackAnim.width = 5;
			attackAnim.height = 2;
			attackAnim.begin = 0;
			attackAnim.end = 9;
		}
		else {
			attackAnim.texNo = attack2TexNo;
			attackAnim.width = 5;
			attackAnim.height = 6;
			attackAnim.begin = 0;
			attackAnim.end = 29;
			rot -= 180.0f * MATH::Deg2Rad;
		}

		attackAnim.Draw(pos + localPos, rot, scl, col);
	}
}
void ClientThunder::DrawUI(void) {
	Vector2 localPos = Vector2(0, player->transform.scale.y * 0.5f + 3);
	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	Vector2 scl = Vector2(9, 9);

	int num = mp;
	int maxNum = state->maxMp;
	int width = 10.0f;
	
	float x = -maxNum * 0.5f + 0.5f;
	for (int i = 0; i < maxNum; i++) {
		if (i < num) DrawSprite(uiTexNo, pos + localPos + Vector2(width, 0.0f) * x, 0.0f, scl, Color::White);
		DrawSprite(frameUITexNo, pos + localPos + Vector2(width, 0.0f) * x, 0.0f, scl, Color::White);
		x += 1.0f;
	}
}

void ServerThunderAttack::Loop(void) {
	// �d��
	velocity += Vector2::Down * gravity;

	// �ړ�
	transform.position += velocity;

	// �ǂɐG�ꂽ�Ȃ�폜
	int id = MultiPlayServer::GetGameMode()->GetMap()->Collision(transform.position, radius * 0.25f);
	if (id != -1) {
		Destroy();
	}
}
void ServerThunderAttack::KnockBack(ServerMovableGameObject *object) {
	object->blownVelocity = self->Cast<ServerPlayer>()->attackVelocity * knockbackRate;
}

void ClientThunderAttack::Loop(void) {
	if (!isShow) return;

	Vector2 pos = transform.position;
	float rot = atan2f(-velocity.y, velocity.x);
	Vector2 scl = Vector2::One * 150.0f;
	Color col = Color::White;
	anim.Draw(pos - MultiPlayClient::offset, rot, scl, col);

	isShow = false;
}
void ClientThunderAttack::Release(void) {
	MultiPlayClient::GetGameMode()->GetMap()->GetEffects()->AddEffect(deathAnim, transform.position, 0.0f, transform.scale);
}
void ClientThunder2Attack::Loop(void) {
	if (!isShow) return;

	Vector2 pos = transform.position;
	float rot = atan2f(velocity.y, -velocity.x);
	Vector2 scl = Vector2::One * 150.0f;
	Color col = Color::White;
	anim.Draw(pos - MultiPlayClient::offset, rot, scl, col);

	isShow = false;
}





/*******************************************************
  Wind
********************************************************/
bool ServerWind::StickTrigger(Vector2 stick, Vector2 previousStick) {
	float stickDistance = stick.Distance();
	float preStickDistance = previousStick.Distance();

	if (state->minInputSpeed < MATH::Abs(Vector2::Cross(stick * 10.f, previousStick)) &&
		state->minInputDistance < stickDistance && state->minInputDistance < preStickDistance) {
		return true;
	}
	return false;
}
void ServerWind::Move(void) {
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);

	// ��]�̃X�s�[�h���擾
	float rotSpeed = Vector2::Cross(stick, previousStick);

	// ����
	if (StickTrigger(stick, previousStick)) {
		// �p���[���Z
		AddPower(rotSpeed);
	}
	else {
		// �񂵂Ă��Ȃ��Ƃ��͂���Ɍ���
		power *= state->brakeFriction;
	}




	// �p���[�̐�Βl
	float absPower = MATH::Abs(power);

	// �ړ���
	if (state->minActionPower < absPower) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimMove(player->animType);

		// �ړ�
		player->velocity = absPower * Vector2::Up * state->powerMoveRatio;;

		// �d�͂��Ȃ���
		player->gravityVelocity = Vector2::Zero;

		// ���ړ��ł��Ȃ��悤�ɂ���
		horizontalVelocity = 0.0f;
	}
	// ����
	else if (0.0f > player->gravityVelocity.y) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimNoMove(player->animType);

		horizontalVelocity += stick.x;
		if (maxHorizontalVelocity < horizontalVelocity) horizontalVelocity = maxHorizontalVelocity;
		else if (horizontalVelocity < -maxHorizontalVelocity) horizontalVelocity = -maxHorizontalVelocity;

		// �ړ�
		player->velocity.x = horizontalVelocity;

		// ����
		horizontalVelocity *= state->friction;
	}

	FrictionPower();
	Friction();
}
void ServerWind::Attack(void) {
	using namespace PHYSICS;
	Vector2 stick = Input::GetStickRight(0);
	Vector2 previousStick = Input::GetPreviousStickRight(0);

	// ���ړ��ł��Ȃ��悤�ɂ���
	horizontalVelocity = 0.0f;

	// ��]�̃X�s�[�h���擾
	float rotSpeed = MATH::Abs(Vector2::Cross(stick, previousStick));

	// ����
	if (StickTrigger(stick, previousStick) && IsUseMp()) {
		// �p���[���Z
		AddPower(rotSpeed);
	}
	else {
		// �񂵂Ă��Ȃ��Ƃ��͂���Ɍ���
		power *= state->brakeFriction;
	}




	// �p���[�̐�Βl
	float absPower = MATH::Abs(power);

	// �U����
	if (state->minActionPower < absPower) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimAttack(player->animType);

		// �U���I�u�W�F�N�g�̐���
		CreateAttack();

		// �A�^�b�N�ړ�
		if (attack_) {
			attack_->transform.position = player->transform.position;
		}
	}
	else {
		// �A�j���[�V�����̎w��
		SetPlayerAnimNoAttack(player->animType);

		// �U���I�u�W�F�N�g�̍폜
		DestroyAttack();
	}

	FrictionPower();
	player->attackVelocity = stick;
}
AttackServerSide *ServerWind::CreateAttack(void) {
	// �U���I�u�W�F�N�g����������Ă���Ȃ�I��
	if (attack_) return nullptr;
	// �N�[���^�C�����Ȃ�I��
	if (state->coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < state->coolTime) return nullptr;



	// �U������
	attack_ = player->map->GetAttacks()->Add<ServerWindAttack>(player, this);

	// �N�[���^�C���v���J�n
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientWind::Move(void) {
	// �ړ��J�n
	if (!IsPlayerAnimMove(prevAnimType) && IsPlayerAnimMove(player->animType)) {
		moveAnim.MoveBegin();
		prevPosition = player->transform.position;
	}

	// ���݂̃A�j���[�V�����^�C�v���L�^
	prevAnimType = player->animType;

	// �ړ��A�j���[�V�����ł͂Ȃ��Ȃ�I��
	if (!IsPlayerAnimMove(player->animType)) return;

	Vector2 pos = player->transform.position;
	float rot = 0.0f;
	Vector2 scl = Vector2(state->showMoveX, state->showMoveY);
	Color col = Color::White;
	// �ړ�
	moveAnim.Draw(prevPosition - MultiPlayClient::offset, rot, scl, col);
}
void ClientWind::Attack(void) {
	// �U���A�j���[�V�����ł͂Ȃ��Ȃ�I��
	if (!IsPlayerAnimAttack(player->animType)) return;



	// ���x���ɂ���ăe�N�X�`���̕ύX
	if (player->GetLv() < 6) {
		attackAnim.texNo = attackTexNo;
		attackAnim.width = 5;
		attackAnim.height = 6;
		attackAnim.end = 29;
	}
	else {
		attackAnim.texNo = attack2TexNo;
		attackAnim.width = 5;
		attackAnim.height = 6;
		attackAnim.end = 29;
	}

	Vector2 pos = player->transform.position;
	float rot = 0.0f;
	Vector2 scl = Vector2(state->showAttackX, state->showAttackY);
	Color col = Color::White;
	attackAnim.Draw(pos - MultiPlayClient::offset, rot, scl, col);
}
void ClientWind::Idle(void) {
	float localScale = 100.0f;


	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	float rot = 0.0f;
	Vector2 scl = Vector2::One * localScale;
	Color col = Color::White;

	// �ҋ@
	if (player->animType == ANIMATION_TYPE_IDLE) {
		idle.Draw(pos + Vector2(0.0f, -70.0f), rot, scl, col);
	}
}
void ClientWind::DrawUI(void) {
	Vector2 localPos = Vector2(player->transform.scale.y * 0.15f, player->transform.scale.y * 0.5f);
	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	Vector2 scl = Vector2::One * 30.0f;

	float ratio = (float)mp / (float)state->maxMp;
	DrawSpriteCircleEffect(frameUITexNo, pos + localPos, 0.0f, scl, Color::White, Vector2::Zero, Vector2::One, 1.0f);
	DrawSpriteCircleEffect(uiTexNo, pos + localPos, 0.0f, scl, Color::White, Vector2::Zero, Vector2::One, ratio);
}

void ServerWindAttack::Loop(void) {

}
void ServerWindAttack::KnockBack(ServerMovableGameObject *object) {
	object->blownVelocity = (object->transform.position - transform.position).Normalize() * knockbackRate;
}

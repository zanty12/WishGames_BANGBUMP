#include "lib/math.h"
#include "multi_attribute.h"
#include "multiplay.h"
#include "lib/collider2d.h"
#include "xinput.h"
#include "time.h"


AttackServerSide *ServerAttribute::CreateAttack(void) {
	return nullptr;
};
void ServerAttribute::DestroyAttack(void) {
	// �U���폜
	if (attack_ && state->atkAfterTime <= atkAfterTimer.GetNowTime() * 0.001f) {
		attack_->Destroy();
		attack_ = nullptr;
	}
};
void ServerAttribute::LevelUpdate(void) {
	int lv = 9;
	// ���x��������̏ꍇ
	if (lvupPoint[lv] <= player->skillPoint) {
		state = &state_lv[lv];
		this->lv = lv;
	}

	// ���x���̒���
	for (lv = 0; lv < 9; lv++) {
		if (lvupPoint[lv] <= player->skillPoint && player->skillPoint < lvupPoint[lv + 1]) {
			state = &state_lv[lv];
			this->lv = lv;
		}
	}
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
ClientAttribute *ClientAttribute::Create(ClientPlayer*player, ATTRIBUTE_TYPE type) {
	switch (type) {
	case ATTRIBUTE_TYPE_FIRE: return new ClientFire(player);
	case ATTRIBUTE_TYPE_DARK: return new ClientWater(player);
	case ATTRIBUTE_TYPE_THUNDER: return new ClientThunder(player);
	case ATTRIBUTE_TYPE_WIND: return new ClientWind(player);
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
		velocity += CalcVector(stick);

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
		if (attack_) {
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
	attack_ = player->map->GetAttacks()->Add<ServerWaterAttack>(player, this);

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
		if (5 < moveAnims.size()) {
			moveAnims.pop_back();
		}
	}
}
void ClientFire::Attack(void) {
	// �U���A�j���[�V�����ł͂Ȃ��Ȃ�I��
	if (!IsPlayerAnimAttack(player->animType)) return;



	// �U���̌���
	Vector2 direction = player->attackVelocity.Normalize() * state->showAttackY * 0.5f;

	// �L�����𔽓]������
	if (direction.x < 0.0f) player->isReverseX = false;
	else if (direction.x > 0.0f) player->isReverseX = true;

	// ��Ɉړ�
	Vector2 localPos = Vector2(-12.0f, 45.0f);

	// �g�����X�t�H�[�}�[
	Vector2 pos = player->transform.position;
	float rot = atan2f(direction.x, direction.y);
	Vector2 scl = Vector2(state->showAttackX, state->showAttackY);

	// ���]�����G�ɍ��킹�Ĉʒu�����]
	if (player->isReverseX) localPos.x *= -1.0f;

	// �`�悷��
	attackAnim.Draw(pos + direction + localPos - MultiPlayClient::offset, rot, scl, Color::White);
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
		player->transform.position = player->warpVelocity;

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
		player->warpVelocity = player->transform.position + CalcVector(stick);

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

	// �U�����ĂȂ�
	if (attack_ == nullptr) {
		// �U��
		if (Input::GetKey(0, Input::RThumb)) {
			// �A�j���[�V�����̎w��
			SetPlayerAnimAttack(player->animType);

			// �U���I�u�W�F�N�g�̐���
			CreateAttack();
		}
		// �`���[�W
		else if (stick != Vector2::Zero) {
			// �A�j���[�V�����̎w��
			SetPlayerAnimAttack(player->animType, true);

			// ���[�v�x�N�g���̎w��
			player->attackVelocity = stick.Normalize();

			// �p���[���Z
			AddPower(stick);
		}
		else {
			// ������
			power = 0.0f;
		}
	}

	// �U����
	if (attack_) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimAttack(player->animType);

		// �A�^�b�N�ړ�
		attack_->transform.position = player->transform.position;
		attack_->direction = player->attackVelocity * state->atkDistance;

		// ���C��R
		player->velocity *= state->friction;
	}

	// �U���I�u�W�F�N�g�̍폜
	if (!Input::GetKey(0, Input::RThumb)) {
		DestroyAttack();

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
				player->warpVelocity - MultiPlayClient::offset,
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
	// �U���A�j���[�V�����ł͂Ȃ��Ȃ�I��
	if (!IsPlayerAnimAttack(player->animType)) return;




	// �U���̌���
	Vector2 direction = player->attackVelocity.Normalize() * state->showAttackY * 0.5f;

	// �L�����𔽓]������
	if (direction.x < 0.0f) player->isReverseX = false;
	else if (direction.x > 0.0f) player->isReverseX = true;

	// ��Ɉړ�
	Vector2 localPos = Vector2(-12.0f, 35.0f);

	// �g�����X�t�H�[�}�[
	Vector2 pos = player->transform.position;
	float rot = atan2f(direction.x, direction.y);
	Vector2 scl = Vector2(state->showAttackX, state->showAttackY);

	// ���]�����G�ɍ��킹�Ĉʒu�����]
	if (player->isReverseX) localPos.x *= -1.0f;

	// �`�悷��
	attackAnim.Draw(pos + direction + localPos - MultiPlayClient::offset, rot, scl, Color::White);
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

	//Charge
	if (state->minInputDistance <= distance) {
		// �p���[���Z
		AddPower(stick);
	}

	//Release
	if (distance < state->minInputDistance && state->minInputDistance < previousDistance &&
		state->minInputSpeed < (stick - previousStick).Distance()) {
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
	}
	// �ړ����I�������i�ړ��I���j
	else if (IsPlayerAnimMove(player->animType) && player->velocity.DistanceSq() < 25.0f) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimNoMove(player->animType);
	}

	// �ړ�
	if (StickTrigger(stick, previousStick)) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimMove(player->animType);

		Vector2 direction = (stick - previousStick).Normalize();
		player->velocity = CalcVector(direction);

		// ������
		power = 0.0f;
	}
	
	Friction();
}
void ServerThunder::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);
	Vector2 previousStick = Input::GetPreviousStickRight(0);

	// �`���[�W��
	if (stick != Vector2::Zero && 0.0f < power) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimAttack(player->animType, true);
	}
	// �U�����I�������i�U���I���j
	else if (IsPlayerAnimMove(player->animType) && player->velocity.DistanceSq() < 25.0f) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimNoAttack(player->animType);
	}

	// �U��
	if (StickTrigger(stick, previousStick)) {
		// �A�j���[�V�����̎w��
		SetPlayerAnimAttack(player->animType);

		Vector2 direction = (stick - previousStick).Normalize();


		// �U���I�u�W�F�N�g�̐���
		auto attack = CreateAttack();

		// �A�^�b�N�ړ�
		if (attack) {
			attack->velocity = CalcVector(direction);
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
	attack_ = player->map->GetAttacks()->Add<ServerThunderAttack>(player, this);

	// �N�[���^�C���v���J�n
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientThunder::Move(void) {

}
void ClientThunder::Attack(void) {

}

void ServerThunderAttack::Loop(void) {
	// �d��
	velocity += Vector2::Down * gravity;

	// �ړ�
	transform.position += velocity;

	// �ǂɐG�ꂽ�Ȃ�폜
	if (MultiPlayServer::GetGameMode()->GetMap()->Collision(transform.position, radius) != -1)
		Destroy();
}
void ServerThunderAttack::KnockBack(ServerMovableGameObject *object) {
	object->blownVelocity = (object->transform.position - transform.position) * knockbackRate;
}

void ClientThunderAttack::Loop(void) {
	float localScale = 300;

	Vector2 pos = transform.position;
	float rot = atan2f(velocity.x, velocity.y);
	Vector2 scl = Vector2(localScale, localScale);
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
		player->velocity = absPower * Vector2::Up;

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

void ServerWindAttack::Loop(void) {

}
void ServerWindAttack::KnockBack(ServerMovableGameObject *object) {
	object->blownVelocity = (object->transform.position - transform.position).Normalize() * knockbackRate;
}

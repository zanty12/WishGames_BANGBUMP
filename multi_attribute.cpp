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
	if (attack_ && atkAfterTime <= atkAfterTimer.GetNowTime() * 0.001f) {
		attack_->Destroy();
		attack_ = nullptr;
	}
};

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
	power += addPower;
	if (maxPower < power) power = maxPower;
	else if (power < minPower) power = minPower;
}
void ServerAttribute::AddPower(float scaler) {
	power += scaler * inputPowerRate;
	if (maxPower < power) power = maxPower;
	else if (power < minPower) power = minPower;
}
void ServerAttribute::AddPower(Vector2 vector) {
	power += addPower + vector.Distance() * inputPowerRate;
	if (maxPower < power) power = maxPower;
	else if (power < minPower) power = minPower;
}
void ServerAttribute::Friction(void) {
	player->velocity.x *= friction;
	player->velocity.y *= friction;
}
Vector2 ServerAttribute::CalcVector(Vector2 stick) {
	return stick * power;
}










/*******************************************************
  Fire
********************************************************/
bool ServerFire::StickTrigger(Vector2 stick, Vector2 previousStick) {
	return stick.DistanceSq() > minInputDistance * minInputDistance;
}

void ServerFire::Move(void) {
	Vector2 stick = Input::GetStickLeft(0);


	// �ړ���
	if (StickTrigger(stick)) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_MOVE;

		// �p���[���Z
		AddPower(stick);

		// �x�N�g���v�Z
		velocity += CalcVector(stick);

		// �d�͂��Ȃ���
		player->gravityVelocity = Vector2::Zero;

		// �x�N�g���̍ő�l
		if (maxPower * maxPower < velocity.DistanceSq()) velocity = velocity.Normalize() * maxPower;
	}
	// ��~��
	else {
		player->animType = ANIMATION_TYPE_IDLE;

		velocity *= friction;
	}
	player->velocity = velocity;

	// ���C
	Friction();
}
void ServerFire::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);


	// �U����
	if (StickTrigger(stick)) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_ATTACK;

		player->attackVelocity = stick;

		// �U���I�u�W�F�N�g�̐���
		CreateAttack();

		// �A�^�b�N�ړ�
		if (attack_) {
			attack_->transform.position = player->transform.position;
			attack_->direction = stick.Normalize() * atkDistance;
			attack_->transform.rotation = std::atan2(stick.y, stick.x);
		}

	}
	else if (attack_ != nullptr) {
		// �U���I�u�W�F�N�g�̍폜
		DestroyAttack();
	}
}
AttackServerSide *ServerFire::CreateAttack(void) {
	// �U���I�u�W�F�N�g����������Ă���Ȃ�I��
	if (attack_) return nullptr;
	// �N�[���^�C�����Ȃ�I��
	if (coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < coolTime) return nullptr;



	// �U������
	attack_ = player->map->GetAttacks()->Add<ServerWaterAttack>(player, this);

	// �N�[���^�C���v���J�n
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientFire::Move(void) {
	// �ړ��A�j���[�V�����ł͂Ȃ��Ȃ�I��
	if (player->animType != ANIMATION_TYPE_MOVE) {
		if (moveAnims.size()) moveAnims.clear();
		return;
	}



	Vector2 direction = player->velocity;
	float localScale = 100;

	// �`�悷��
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
		Vector2 scl = Vector2::One * localScale;
		Color col = Color::White;
		moveAnims.push_front({ pos, rot, scl, moveAnim });

		// �v�f�������Ȃ�폜
		if (10 < moveAnims.size()) {
			moveAnims.pop_back();
		}
	}
}
void ClientFire::Attack(void) {
	// �U���A�j���[�V�����ł͂Ȃ��Ȃ�I��
	if (player->animType != ANIMATION_TYPE_ATTACK) {
		if (attackAnims.size()) attackAnims.clear();
		return;
	}


	Vector2 direction = player->attackVelocity;
	float localScale = 100;

	// �`�悷��
	float denominator = attackAnims.size();
	float numerator = denominator * 0.5f;
	for (Animator &anim : attackAnims) {
		anim.anim.Draw(anim.pos - MultiPlayClient::offset, anim.rot, anim.scl, Color(1.0f, 1.0f, 1.0f, 1.0f - numerator / denominator));
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
		float distance = 50.0f;
		Vector2 pos = player->transform.position + direction.Normalize() * distance;
		float rot = atan2f(direction.x, direction.y/*direction.y, -direction.x*/);
		Vector2 scl = Vector2::One * localScale;
		Color col = Color::White;
		attackAnims.push_front({ pos, rot, scl, attackAnim });

		// �v�f�������Ȃ�폜
		if (10 < attackAnims.size()) {
			attackAnims.pop_back();
		}
	}
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
		player->animType = ANIMATION_TYPE_MOVE;

		// �ړ�
		player->transform.position = player->warpVelocity;

		// ������
		power = 0.0f;
	}
	// ���[�v�����̃`���[�W
	else if (stick != Vector2::Zero) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_MOVE_CHARGE;

		// �p���[���Z
		AddPower(stick);

		// ���[�v�x�N�g���̎w��
		player->warpVelocity = player->transform.position + CalcVector(stick);

		// �d�͂����X�ɂȂ���
		player->gravityVelocity *= friction;
	}
	else {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_IDLE;

		power = 0.0f;
	}

	player->velocity *= friction;
}
void ServerWater::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);

	// �U�����ĂȂ�
	if (attack_ == nullptr) {
		// �U��
		if (Input::GetKey(0, Input::RThumb)) {
			// �A�j���[�V�����̎w��
			player->animType = ANIMATION_TYPE_ATTACK;

			// �U���I�u�W�F�N�g�̐���
			CreateAttack();
		}
		// �`���[�W
		else if (stick != Vector2::Zero) {
			// �A�j���[�V�����̎w��
			player->animType = ANIMATION_TYPE_ATTACK_CHARGE;

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
		player->animType = ANIMATION_TYPE_ATTACK;

		// �A�^�b�N�ړ�
		attack_->transform.position = player->transform.position;
		attack_->direction = player->attackVelocity * atkDistance;

		// ���C��R
		player->velocity *= friction;
	}

	// �U���I�u�W�F�N�g�̍폜
	if (!Input::GetKey(0, Input::RThumb)) {
		DestroyAttack();
	}
}
AttackServerSide *ServerWater::CreateAttack(void) {
	// �U���I�u�W�F�N�g����������Ă���Ȃ�I��
	if (attack_) return nullptr;
	// �N�[���^�C�����Ȃ�I��
	if (coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < coolTime) return nullptr;



	// �U������
	attack_ = player->map->GetAttacks()->Add<ServerWaterAttack>(player, this);

	// �N�[���^�C���v���J�n
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientWater::Move(void) {
	float localScale = 100.0f;

	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	float rot = 0.0f;
	Vector2 scl = Vector2::One * localScale;
	Color col = Color::White;

	// �ړ�
	if (player->animType == ANIMATION_TYPE_MOVE) {
		moveAnim.MoveBegin();
	}
	// �`���[�W
	else if (player->animType == ANIMATION_TYPE_MOVE_CHARGE) {
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

	moveAnim.Draw(prevPosition - MultiPlayClient::offset + Vector2(0.0f, localScale), rot, scl * 2.0f, col);
	moveAnim.Draw(player->transform.position - MultiPlayClient::offset + Vector2(0.0f, localScale), rot, scl * 2.0f, col);
}
void ClientWater::Attack(void) {
	// �U���A�j���[�V�����ł͂Ȃ��Ȃ�I��
	if (player->animType != ANIMATION_TYPE_ATTACK) return;



	Vector2 direction = player->attackVelocity;
	float localScale = 100;

	// �A�j���[�V��������
	float distance = 1000.0f;
	Vector2 pos = player->transform.position + direction.Normalize() * distance * 0.5f;
	float rot = atan2f(direction.x, direction.y);
	Vector2 scl = Vector2(localScale, distance);
	Color col = Color::White;
	attackAnim.Draw(pos - MultiPlayClient::offset, rot, scl, col);
}
void ClientWater::Idle(void) {
	float localScale = 100.0f;


	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	float rot = 0.0f;
	Vector2 scl = Vector2::One * localScale;
	Color col = Color::White;

	// �ҋ@
	if (player->animType == ANIMATION_TYPE_IDLE) {
		idle.Draw(pos + Vector2(0.0f, -60.0f), rot, scl, col);
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
	if (minInputDistance <= distance) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_MOVE_CHARGE;

		// �p���[���Z
		AddPower(stick);
	}

	//Release
	if (distance < minInputDistance && minInputDistance < previousDistance &&
		minInputSpeed < (stick - previousStick).Distance()) {
		return true;
	}
	return false;
}
void ServerThunder::Move(void) {
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);

	if (StickTrigger(stick, previousStick)) {
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

	if (StickTrigger(stick, previousStick)) {
		Vector2 direction = (stick - previousStick).Normalize();


		// �U���I�u�W�F�N�g�̐���
		auto attack = CreateAttack();
		//auto attack = player->map->GetAttacks()->Add<ServerThunderAttack>(player, this);

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
	if (coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < coolTime) return nullptr;



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
	float localScale = 100;

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

	if (minInputSpeed < MATH::Abs(Vector2::Cross(stick, previousStick)) &&
		minInputDistance < stickDistance && minInputDistance < preStickDistance) {
		return true;
	}
	return false;
}
void ServerWind::Move(void) {
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);

	// ��]�̃X�s�[�h���擾
	float rotSpeed = MATH::Abs(Vector2::Cross(stick, previousStick));

	// �ړ���
	if (StickTrigger(stick, previousStick))
	{
		player->animType = ANIMATION_TYPE_MOVE;

		// �p���[���Z
		AddPower(rotSpeed);

		// �ړ�
		player->velocity = CalcVector(Vector2::Up);
		previous_time_ = Time::GetCurrentTime();

		// �d�͂��Ȃ���
		player->gravityVelocity = Vector2::Zero;
	}
	// ��������
	else if (0 > player->velocity.y || prev_y_ > player->velocity.y || Time::GetDeltaTime(previous_time_) > 0.04f)
	{
		power *= powerFriction;

		player->velocity.x = stick.x * 6 * Time::GetDeltaTime();
		//��ړ��̊����c���Ă���ꍇ
		if (player->velocity.y > 0)
			player->velocity.y -= player->velocity.y * 2 * Time::GetDeltaTime();
	}
	else if (stick.Distance() < minInputSpeed)
	{
		power *= powerFriction;
	}
	prev_y_ = player->velocity.y;
}
void ServerWind::Attack(void) {
	using namespace PHYSICS;
	Vector2 stick = Input::GetStickRight(0);
	Vector2 previousStick = Input::GetPreviousStickRight(0);


	// ��]�̃X�s�[�h���擾
	float rotSpeed = MATH::Abs(Vector2::Cross(stick, previousStick));

	// �U����
	if (StickTrigger(stick, previousStick)) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_ATTACK;

		// �U���I�u�W�F�N�g�̐���
		CreateAttack();

		// �A�^�b�N�ړ�
		if (attack_) {
			attack_->transform.position = player->transform.position;
		}
	}
	else {
		// �U���I�u�W�F�N�g�̍폜
		DestroyAttack();
	}

	player->attackVelocity = stick;
}
AttackServerSide *ServerWind::CreateAttack(void) {
	// �U���I�u�W�F�N�g����������Ă���Ȃ�I��
	if (attack_) return nullptr;
	// �N�[���^�C�����Ȃ�I��
	if (coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < coolTime) return nullptr;



	// �U������
	attack_ = player->map->GetAttacks()->Add<ServerWindAttack>(player, this);

	// �N�[���^�C���v���J�n
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientWind::Move(void) {
	// �ړ��A�j���[�V�����ł͂Ȃ��Ȃ�I��
	if (player->animType != ANIMATION_TYPE_MOVE) return;
	float localScale = 200;



	Vector2 pos = player->transform.position;
	float rot = 0.0f;
	Vector2 scl = Vector2::One * localScale;
	Color col = Color::White;
	// �ړ�
	if (player->animType == ANIMATION_TYPE_MOVE) {
		moveAnim.Draw(pos - MultiPlayClient::offset, rot, scl, col);
	}
}
void ClientWind::Attack(void) {
	// �U���A�j���[�V�����ł͂Ȃ��Ȃ�I��
	if (player->animType != ANIMATION_TYPE_ATTACK) return;
	float localScale = 200;


	Vector2 pos = player->transform.position;
	float rot = 0.0f;
	Vector2 scl = Vector2::One * localScale;
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
		idle.Draw(pos + Vector2(0.0f, -60.0f), rot, scl, col);
	}
}

void ServerWindAttack::Loop(void) {

}
void ServerWindAttack::KnockBack(ServerMovableGameObject *object) {
	object->blownVelocity = (object->transform.position - transform.position).Normalize() * knockbackRate;
}

#include "lib/math.h"
#include "multi_attribute.h"
#include "multiplay.h"
#include "lib/collider2d.h"
#include "xinput.h"
#include "time.h"


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

		// �x�N�g���̍ő�l
		if (maxPower * maxPower < velocity.DistanceSq()) velocity = velocity.Normalize() * maxPower;
	}
	// ��~��
	else {
		player->animType = ANIMATION_TYPE_IDEL;
	}
	player->velocity = velocity;

	// ���C
	Friction();
}
void ServerFire::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);


	// �ړ���
	if (StickTrigger(stick)) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_ATTACK;

		player->attackVelocity = stick;

		// �U���I�u�W�F�N�g�̐���
		if (attack_ == nullptr)
			attack_ = player->map->GetAttacks()->Add<ServerFireAttack>(player);
		attack_->transform.position = player->transform.position;
		attack_->transform.rotation = std::atan2(stick.y, stick.x);
	}
	else if (attack_ != nullptr) {
		attack_->Destroy();
		attack_ = nullptr;
	}
}

void ClientFire::Move(void) {
	Vector2 direction = player->velocity;
	float localScale = 100;

	// �`�悷��
	float denominator = moveAnims.size();
	float numerator = denominator * 0.5f;
	for (Animator &anim : moveAnims) {
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




/*******************************************************
  Water
********************************************************/
bool ServerWater::StickTrigger(Vector2 stick, Vector2 previousStick) {


	return false;
}
void ServerWater::Move(void) {
	Vector2 stick = Input::GetStickLeft(0);


	// �A�j���[�V�����̎w��
	player->animType = ANIMATION_TYPE_IDEL;

	// ���[�v�����̃`���[�W
	if (Input::GetKey(0, Input::LThumb)) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_MOVE_CHARGE;

		// �p���[���Z
		AddPower(stick);

		// ���[�v�x�N�g���̎w��
		player->warpVelocity = player->transform.position + CalcVector(stick);
	}
	// ���[�v
	else if(Input::GetKeyUp(0, Input::LThumb)) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_MOVE;

		// �ړ�
		player->transform.position = player->warpVelocity;

		// ������
		power = 0.0f;
	}
}
void ServerWater::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);


	// �U��
	if (Input::GetKeyDown(0, Input::RThumb)) {

		// ���[�v�x�N�g���̎w��
		player->attackVelocity = CalcVector(stick);

		// �U���I�u�W�F�N�g�̐���
		if (attack_ == nullptr)
			attack_ = player->map->GetAttacks()->Add<ServerWaterAttack>(player);
		attack_->direction = stick * 100.0f;
	}
	if (Input::GetKey(0, Input::RThumb)) {

		// �ړ��`���[�W�A�j���[�V����
		player->animType = ANIMATION_TYPE_ATTACK;

		// �U���ꏊ�̍X�V
		attack_->transform.position = player->transform.position;

	}
	// �U���I��
	else if (Input::GetKeyUp(0, Input::RThumb)) {
		// ������
		power = 0.0f;

		if (attack_) {
			// �U���폜
			attack_->Destroy();
			attack_ = nullptr;
		}
	}
}

void ClientWater::Move(void) {
	float localScale = 100.0f;

	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	float rot = 0.0f;
	Vector2 scl = Vector2::One * localScale;
	Color col = Color::White;

	if (player->animType == ANIMATION_TYPE_MOVE) {
		// �ړ�
		moveAnim.MoveBegin();
	}
	else if (player->animType == ANIMATION_TYPE_MOVE_CHARGE) {
		moveChargeAnim.Draw(pos, rot, scl, col);

		player->anim.Draw(
			player->warpVelocity - MultiPlayClient::offset,
			player->transform.rotation,
			player->transform.scale,
			Color(1.0f, 1.0f, 1.0f, 0.5f)
		);

		// ���ݒn���L�^
		if(moveAnim.IsEnd()) prevPosition = player->transform.position;
	}


	moveAnim.Draw(prevPosition - MultiPlayClient::offset, rot, scl, col);
}
void ClientWater::Attack(void) {
	Vector2 direction = player->attackVelocity;
	float localScale = 100;

	// �A�j���[�V��������
	float distance = 50.0f;
	Vector2 pos = player->transform.position + direction.Normalize() * distance;
	float rot = atan2f(direction.x, direction.y);
	Vector2 scl = Vector2::One * localScale;
	Color col = Color::White;
	attackAnim.Draw(pos - MultiPlayClient::offset, rot, scl, col);
}

void ServerWaterAttack::Loop(void) {

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
		// ������
		power = 0.0f;
		return true;
	}
	return false;
}
void ServerThunder::Move(void) {
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);

	if (StickTrigger(stick, previousStick)) {
		player->velocity = (stick - previousStick).Normalize() * power;
	}

	Friction();
}
void ServerThunder::Attack(void) {

}

void ClientThunder::Move(void) {

}
void ClientThunder::Attack(void) {

}






/*******************************************************
  Wind
********************************************************/
bool ServerWind::StickTrigger(Vector2 stick, Vector2 previousStick)
{
	float stickDistance = stick.Distance();
	float preStickDistance = previousStick.Distance();

	if (minInputSpeed < MATH::Abs(Vector2::Cross(stick, previousStick)) &&
		minInputDistance < stickDistance && minInputDistance < preStickDistance)
	{
		return true;
	}
	return false;
}
void ServerWind::Move(void) {
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);

	// ��]�̃X�s�[�h���擾
	float rotSpeed = Vector2::Cross(stick, previousStick);

	// �ړ���
	if (StickTrigger(stick, previousStick))
	{
		player->animType = ANIMATION_TYPE_MOVE;

		// �p���[���Z
		AddPower(rotSpeed);

		// �ړ�
		player->velocity = CalcVector(Vector2::Up);
		previous_time_ = Time::GetCurrentTime();
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
	float rotSpeed = Vector2::Cross(stick, previousStick);

	// �U����
	if (StickTrigger(stick, previousStick)) {
		player->animType = ANIMATION_TYPE_ATTACK;
		if (attack_ == nullptr)
			attack_ = player->map->GetAttacks()->Add<ServerWindAttack>(player);
		attack_->transform.position = player->transform.position;
	}
	else if (attack_ != nullptr) {
		attack_->Destroy();
		attack_ = nullptr;
	}

	player->attackVelocity = stick;
}

void ClientWind::Move(void) {
	float localScale = 200;


	Vector2 pos = player->transform.position;
	float rot = 0.0f;
	Vector2 scl = Vector2::One * localScale;
	Color col = Color::White;
	moveAnim.Draw(pos - MultiPlayClient::offset, rot, scl, col);
}
void ClientWind::Attack(void) {
	float localScale = 200;


	Vector2 pos = player->transform.position;
	float rot = 0.0f;
	Vector2 scl = Vector2::One * localScale;
	Color col = Color::White;
	attackAnim.Draw(pos - MultiPlayClient::offset, rot, scl, col);
}


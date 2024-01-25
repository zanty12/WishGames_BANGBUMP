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

/*******************************************************
  Fire
********************************************************/
bool ServerFire::StickTrigger(Vector2 stick, Vector2 previousStick) {
	return false;
}

void ServerFire::Move(void) {
	Vector2 stick = Input::GetStickLeft(0);

	// �ړ���
	if (stick.Distance() > judgeScale) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_MOVE;

		velocity += stick;
		if (velocity.Distance() > maxSpeed) velocity = velocity.Normalize() * maxSpeed;
	}
	// ��~��
	else {
		player->animType = ANIMATION_TYPE_IDEL;
	}
	velocity.x *= friction;
	velocity.y *= friction;
	player->velocity = velocity;
}
void ServerFire::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);

	// �ړ���
	if (stick.DistanceSq() > judgeScale * judgeScale) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_ATTACK;

		player->attackVelocity = stick;
	}
	velocity.x *= friction;
	velocity.y *= friction;
	player->velocity = velocity;
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
		float distance = 50.0f;
		Vector2 pos = player->transform.position + -direction.Normalize() * distance;
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





/*******************************************************
  Water
********************************************************/
bool ServerWater::StickTrigger(Vector2 stick, Vector2 previousStick) {


	return false;
}
void ServerWater::Move(void) {
	// �A�j���[�V�����̎w��
	player->animType = ANIMATION_TYPE_IDEL;

	// ���[�v�����̃`���[�W
	if (Input::GetKey(0, Input::LThumb)) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_MOVE_CHARGE;

		warpDistance += charge;

		// ���[�v�������ő勗���ɂ���
		if (maxDistance < warpDistance) warpDistance = maxDistance;

		// ���[�v�x�N�g���̎w��
		player->warpVelocity = player->transform.position + Input::GetStickLeft(0) * warpDistance;
	}
	// ���[�v
	else if(Input::GetKeyUp(0, Input::LThumb)) {
		// �A�j���[�V�����̎w��
		player->animType = ANIMATION_TYPE_MOVE;

		player->transform.position = player->warpVelocity;
	}
}
void ServerWater::Attack(void) {
	// ���[�v�����̃`���[�W
	if (Input::GetKey(0, Input::RThumb)) {
		warpDistance += charge;

		// ���[�v�������ő勗���ɂ���
		if (maxDistance < warpDistance) warpDistance = maxDistance;

		// ���[�v�x�N�g���̎w��
		player->warpVelocity = Input::GetStickLeft(0) * warpDistance;

		// �ړ��`���[�W�A�j���[�V����
		player->animType = ANIMATION_TYPE_MOVE_CHARGE;
	}
	// ���[�v
	else if (Input::GetKeyUp(0, Input::RThumb)) {
		player->warpVelocity = player->transform.position + player->warpVelocity;

		warpDistance = 0.0f;
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

}





/*******************************************************
  Thunder
********************************************************/
bool ServerThunder::StickTrigger(Vector2 stick, Vector2 previousStick) {
	return false;
}
void ServerThunder::Move(void) {

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

	if (rotInputJudgeMin < MATH::Abs(Vector2::Cross(stick, previousStick)) &&
		0.8f < stickDistance * preStickDistance)
	{
		return true;
	}
	return false;
}
void ServerWind::Move(void) {
	Vector2 &vel = player->velocity;
	Vector2 stick = Input::GetStickLeft(0);
	Vector2 previousStick = Input::GetPreviousStickLeft(0);

	// ��]�̃X�s�[�h���擾
	float rotSpeed = Vector2::Cross(stick, previousStick);

	// �ړ���
	if (StickTrigger(stick, previousStick))
	{
		player->animType = ANIMATION_TYPE_MOVE;
		power_ += rotSpeed * rotSpeed * rotInputFriction / Time::GetDeltaTime();
		if (maxPower_ < power_) power_ = maxPower_;

		vel = Vector2::Up * power_ * Time::GetDeltaTime();
		previous_time_ = Time::GetCurrentTime();
	}
	// ��������
	else if (0 < Vector2::Dot(Vector2::Down, vel) || prev_y_ > vel.y || Time::GetDeltaTime(previous_time_) > 0.04f)
	{
		power_ *= friction_;

		vel.x = stick.x * 6 * Time::GetDeltaTime();
		//��ړ��̊����c���Ă���ꍇ
		if (vel.y > 0)
			vel.y -= vel.y * 2 * Time::GetDeltaTime();
	}
	else if (stick.Distance() < rotInputJudgeMin)
	{
		power_ *= friction_;
	}
	prev_y_ = vel.y;
}
void ServerWind::Attack(void) {
	using namespace PHYSICS;
	Vector2 stick = Input::GetStickRight(0);
	Vector2 previousStick = Input::GetPreviousStickRight(0);


	// ��]�̃X�s�[�h���擾
	float rotSpeed = Vector2::Cross(stick, previousStick);

	// �U����
	if (StickTrigger(stick, previousStick))
	{
		player->animType = ANIMATION_TYPE_ATTACK;
		if (attack_ == nullptr)
			attack_ = player->map->GetAttacks()->Add<ServerWindAttack>(player);
		attack_->transform.position = player->transform.position;
	}
	else if (attack_ != nullptr)
	{
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

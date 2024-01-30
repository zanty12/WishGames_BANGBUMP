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
	// 攻撃削除
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


	// 移動中
	if (StickTrigger(stick)) {
		// アニメーションの指定
		player->animType = ANIMATION_TYPE_MOVE;

		// パワー加算
		AddPower(stick);

		// ベクトル計算
		velocity += CalcVector(stick);

		// 重力をなくす
		player->gravityVelocity = Vector2::Zero;

		// ベクトルの最大値
		if (maxPower * maxPower < velocity.DistanceSq()) velocity = velocity.Normalize() * maxPower;
	}
	// 停止中
	else {
		player->animType = ANIMATION_TYPE_IDLE;

		velocity *= friction;
	}
	player->velocity = velocity;

	// 摩擦
	Friction();
}
void ServerFire::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);


	// 攻撃中
	if (StickTrigger(stick)) {
		// アニメーションの指定
		player->animType = ANIMATION_TYPE_ATTACK;

		player->attackVelocity = stick;

		// 攻撃オブジェクトの生成
		CreateAttack();

		// アタック移動
		if (attack_) {
			attack_->transform.position = player->transform.position;
			attack_->direction = stick.Normalize() * atkDistance;
			attack_->transform.rotation = std::atan2(stick.y, stick.x);
		}

	}
	else if (attack_ != nullptr) {
		// 攻撃オブジェクトの削除
		DestroyAttack();
	}
}
AttackServerSide *ServerFire::CreateAttack(void) {
	// 攻撃オブジェクトが生成されているなら終了
	if (attack_) return nullptr;
	// クールタイム中なら終了
	if (coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < coolTime) return nullptr;



	// 攻撃生成
	attack_ = player->map->GetAttacks()->Add<ServerWaterAttack>(player, this);

	// クールタイム計測開始
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientFire::Move(void) {
	// 移動アニメーションではないなら終了
	if (player->animType != ANIMATION_TYPE_MOVE) {
		if (moveAnims.size()) moveAnims.clear();
		return;
	}



	Vector2 direction = player->velocity;
	float localScale = 100;

	// 描画する
	float denominator = moveAnims.size();
	float numerator = denominator * 0.5f;
	for (Animator &anim : moveAnims) {
		anim.anim.Draw(anim.pos - MultiPlayClient::offset, anim.rot, anim.scl, Color(1.0f, 1.0f, 1.0f, 1.0f/* - numerator / denominator*/));
		numerator += 0.5f;
	}

	// 時間を計測する
	DWORD currentTime = timeGetTime();
	DWORD deltaTime = currentTime - startTime;

	// アニメーションを作成する
	if (25 < deltaTime) {
		// 計測をリセットする
		startTime = currentTime;

		// 炎の位置をずらす
		direction += direction.Normal() * MATH::Rand(-0.25f, 0.25f);

		// アニメーション生成
		Vector2 pos = player->transform.position + -direction.Normalize();
		float rot = atan2f(direction.y, -direction.x);
		Vector2 scl = Vector2::One * localScale;
		Color col = Color::White;
		moveAnims.push_front({ pos, rot, scl, moveAnim });

		// 要素が多いなら削除
		if (10 < moveAnims.size()) {
			moveAnims.pop_back();
		}
	}
}
void ClientFire::Attack(void) {
	// 攻撃アニメーションではないなら終了
	if (player->animType != ANIMATION_TYPE_ATTACK) {
		if (attackAnims.size()) attackAnims.clear();
		return;
	}


	Vector2 direction = player->attackVelocity;
	float localScale = 100;

	// 描画する
	float denominator = attackAnims.size();
	float numerator = denominator * 0.5f;
	for (Animator &anim : attackAnims) {
		anim.anim.Draw(anim.pos - MultiPlayClient::offset, anim.rot, anim.scl, Color(1.0f, 1.0f, 1.0f, 1.0f - numerator / denominator));
		numerator += 0.5f;
	}

	// 時間を計測する
	DWORD currentTime = timeGetTime();
	DWORD deltaTime = currentTime - startTime;

	// アニメーションを作成する
	if (25 < deltaTime) {
		// 計測をリセットする
		startTime = currentTime;

		// 炎の位置をずらす
		direction += direction.Normal() * MATH::Rand(-0.25f, 0.25f);

		// アニメーション生成
		float distance = 50.0f;
		Vector2 pos = player->transform.position + direction.Normalize() * distance;
		float rot = atan2f(direction.x, direction.y/*direction.y, -direction.x*/);
		Vector2 scl = Vector2::One * localScale;
		Color col = Color::White;
		attackAnims.push_front({ pos, rot, scl, attackAnim });

		// 要素が多いなら削除
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


	// ワープ
	if (Input::GetKeyDown(0, Input::LThumb)) {
		// アニメーションの指定
		player->animType = ANIMATION_TYPE_MOVE;

		// 移動
		player->transform.position = player->warpVelocity;

		// 初期化
		power = 0.0f;
	}
	// ワープ距離のチャージ
	else if (stick != Vector2::Zero) {
		// アニメーションの指定
		player->animType = ANIMATION_TYPE_MOVE_CHARGE;

		// パワー加算
		AddPower(stick);

		// ワープベクトルの指定
		player->warpVelocity = player->transform.position + CalcVector(stick);

		// 重力を徐々になくす
		player->gravityVelocity *= friction;
	}
	else {
		// アニメーションの指定
		player->animType = ANIMATION_TYPE_IDLE;

		power = 0.0f;
	}

	player->velocity *= friction;
}
void ServerWater::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);

	// 攻撃してない
	if (attack_ == nullptr) {
		// 攻撃
		if (Input::GetKey(0, Input::RThumb)) {
			// アニメーションの指定
			player->animType = ANIMATION_TYPE_ATTACK;

			// 攻撃オブジェクトの生成
			CreateAttack();
		}
		// チャージ
		else if (stick != Vector2::Zero) {
			// アニメーションの指定
			player->animType = ANIMATION_TYPE_ATTACK_CHARGE;

			// ワープベクトルの指定
			player->attackVelocity = stick.Normalize();

			// パワー加算
			AddPower(stick);
		}
		else {
			// 初期化
			power = 0.0f;
		}
	}

	// 攻撃中
	if (attack_) {
		// アニメーションの指定
		player->animType = ANIMATION_TYPE_ATTACK;

		// アタック移動
		attack_->transform.position = player->transform.position;
		attack_->direction = player->attackVelocity * atkDistance;

		// 摩擦抵抗
		player->velocity *= friction;
	}

	// 攻撃オブジェクトの削除
	if (!Input::GetKey(0, Input::RThumb)) {
		DestroyAttack();
	}
}
AttackServerSide *ServerWater::CreateAttack(void) {
	// 攻撃オブジェクトが生成されているなら終了
	if (attack_) return nullptr;
	// クールタイム中なら終了
	if (coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < coolTime) return nullptr;



	// 攻撃生成
	attack_ = player->map->GetAttacks()->Add<ServerWaterAttack>(player, this);

	// クールタイム計測開始
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

	// 移動
	if (player->animType == ANIMATION_TYPE_MOVE) {
		moveAnim.MoveBegin();
	}
	// チャージ
	else if (player->animType == ANIMATION_TYPE_MOVE_CHARGE) {
		moveChargeAnim.Draw(pos, rot, scl, col);

		if (player->id == MultiPlayClient::GetID()) {
			// 移動先の描画
			indicator.Draw(
				player->warpVelocity - MultiPlayClient::offset,
				player->transform.rotation,
				player->transform.scale,
				Color(1.0f, 1.0f, 1.0f, 0.5f)
			);
		}

		// 現在地を記録
		if(moveAnim.IsEnd()) prevPosition = player->transform.position;
	}

	moveAnim.Draw(prevPosition - MultiPlayClient::offset + Vector2(0.0f, localScale), rot, scl * 2.0f, col);
	moveAnim.Draw(player->transform.position - MultiPlayClient::offset + Vector2(0.0f, localScale), rot, scl * 2.0f, col);
}
void ClientWater::Attack(void) {
	// 攻撃アニメーションではないなら終了
	if (player->animType != ANIMATION_TYPE_ATTACK) return;



	Vector2 direction = player->attackVelocity;
	float localScale = 100;

	// アニメーション生成
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

	// 待機
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
		// アニメーションの指定
		player->animType = ANIMATION_TYPE_MOVE_CHARGE;

		// パワー加算
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

		// 初期化
		power = 0.0f;
	}

	Friction();
}
void ServerThunder::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);
	Vector2 previousStick = Input::GetPreviousStickRight(0);

	if (StickTrigger(stick, previousStick)) {
		Vector2 direction = (stick - previousStick).Normalize();


		// 攻撃オブジェクトの生成
		auto attack = CreateAttack();
		//auto attack = player->map->GetAttacks()->Add<ServerThunderAttack>(player, this);

		// アタック移動
		if (attack) {
			attack->velocity = CalcVector(direction);
		}

		// 初期化
		power = 0.0f;
	}

	Friction();
}
AttackServerSide *ServerThunder::CreateAttack(void) {
	// クールタイム中なら終了
	if (coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < coolTime) return nullptr;



	// 攻撃生成
	attack_ = player->map->GetAttacks()->Add<ServerThunderAttack>(player, this);

	// クールタイム計測開始
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientThunder::Move(void) {

}
void ClientThunder::Attack(void) {

}

void ServerThunderAttack::Loop(void) {
	// 重力
	velocity += Vector2::Down * gravity;

	// 移動
	transform.position += velocity;

	// 壁に触れたなら削除
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

	// 回転のスピードを取得
	float rotSpeed = MATH::Abs(Vector2::Cross(stick, previousStick));

	// 移動中
	if (StickTrigger(stick, previousStick))
	{
		player->animType = ANIMATION_TYPE_MOVE;

		// パワー加算
		AddPower(rotSpeed);

		// 移動
		player->velocity = CalcVector(Vector2::Up);
		previous_time_ = Time::GetCurrentTime();

		// 重力をなくす
		player->gravityVelocity = Vector2::Zero;
	}
	// 落下処理
	else if (0 > player->velocity.y || prev_y_ > player->velocity.y || Time::GetDeltaTime(previous_time_) > 0.04f)
	{
		power *= powerFriction;

		player->velocity.x = stick.x * 6 * Time::GetDeltaTime();
		//上移動の慣性残っている場合
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


	// 回転のスピードを取得
	float rotSpeed = MATH::Abs(Vector2::Cross(stick, previousStick));

	// 攻撃中
	if (StickTrigger(stick, previousStick)) {
		// アニメーションの指定
		player->animType = ANIMATION_TYPE_ATTACK;

		// 攻撃オブジェクトの生成
		CreateAttack();

		// アタック移動
		if (attack_) {
			attack_->transform.position = player->transform.position;
		}
	}
	else {
		// 攻撃オブジェクトの削除
		DestroyAttack();
	}

	player->attackVelocity = stick;
}
AttackServerSide *ServerWind::CreateAttack(void) {
	// 攻撃オブジェクトが生成されているなら終了
	if (attack_) return nullptr;
	// クールタイム中なら終了
	if (coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < coolTime) return nullptr;



	// 攻撃生成
	attack_ = player->map->GetAttacks()->Add<ServerWindAttack>(player, this);

	// クールタイム計測開始
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientWind::Move(void) {
	// 移動アニメーションではないなら終了
	if (player->animType != ANIMATION_TYPE_MOVE) return;
	float localScale = 200;



	Vector2 pos = player->transform.position;
	float rot = 0.0f;
	Vector2 scl = Vector2::One * localScale;
	Color col = Color::White;
	// 移動
	if (player->animType == ANIMATION_TYPE_MOVE) {
		moveAnim.Draw(pos - MultiPlayClient::offset, rot, scl, col);
	}
}
void ClientWind::Attack(void) {
	// 攻撃アニメーションではないなら終了
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

	// 待機
	if (player->animType == ANIMATION_TYPE_IDLE) {
		idle.Draw(pos + Vector2(0.0f, -60.0f), rot, scl, col);
	}
}

void ServerWindAttack::Loop(void) {

}
void ServerWindAttack::KnockBack(ServerMovableGameObject *object) {
	object->blownVelocity = (object->transform.position - transform.position).Normalize() * knockbackRate;
}

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
	if (attack_ && state->atkAfterTime <= atkAfterTimer.GetNowTime() * 0.001f) {
		attack_->Destroy();
		attack_ = nullptr;
	}
};
void ServerAttribute::LevelUpdate(void) {
	int lv = 9;
	// レベルが上限の場合
	if (lvupPoint[lv] <= player->skillPoint) {
		state = &state_lv[lv];
		this->lv = lv;
	}

	// レベルの調整
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


	// 移動中
	if (StickTrigger(stick)) {
		// アニメーションの指定
		SetPlayerAnimMove(player->animType);

		// パワー加算
		AddPower(stick);

		// ベクトル計算
		velocity += CalcVector(stick);

		// 重力をなくす
		player->gravityVelocity = Vector2::Zero;

		// ベクトルの最大値
		float maxPowerSq = state->maxPower * state->maxPower;
		if (maxPowerSq < velocity.DistanceSq()) velocity = velocity.Normalize() * state->maxPower;

	}
	// 停止中
	else {
		// アニメーションの指定
		SetPlayerAnimNoMove(player->animType);
	}

	velocity *= state->friction;
	player->velocity = velocity;

	// 摩擦
	Friction();
}
void ServerFire::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);


	// 攻撃中
	if (StickTrigger(stick)) {
		// アニメーションの指定
		SetPlayerAnimAttack(player->animType);

		player->attackVelocity = stick;

		// 攻撃オブジェクトの生成
		CreateAttack();

		// アタック移動
		if (attack_) {
			attack_->transform.position = player->transform.position;
			attack_->direction = stick.Normalize() * state->atkDistance;
			attack_->transform.rotation = std::atan2(stick.y, stick.x);
		}

	}
	else if (attack_ != nullptr) {
		// アニメーションの指定
		SetPlayerAnimNoAttack(player->animType);

		// 攻撃オブジェクトの削除
		DestroyAttack();
	}
}
AttackServerSide *ServerFire::CreateAttack(void) {
	// 攻撃オブジェクトが生成されているなら終了
	if (attack_) return nullptr;
	// クールタイム中なら終了
	if (state->coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < state->coolTime) return nullptr;



	// 攻撃生成
	attack_ = player->map->GetAttacks()->Add<ServerWaterAttack>(player, this);

	// クールタイム計測開始
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientFire::Move(void) {
	// 移動アニメーションではないなら終了
	if (!IsPlayerAnimMove(player->animType)) {
		if (moveAnims.size()) moveAnims.clear();
		return;
	}



	// 描画する
	Vector2 direction = player->velocity;
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
		Vector2 scl = Vector2(state->showMoveX, state->showMoveY);
		Color col = Color::White;
		moveAnims.push_front({ pos, rot, scl, moveAnim });

		// 要素が多いなら削除
		if (5 < moveAnims.size()) {
			moveAnims.pop_back();
		}
	}
}
void ClientFire::Attack(void) {
	// 攻撃アニメーションではないなら終了
	if (!IsPlayerAnimAttack(player->animType)) return;



	// 攻撃の向き
	Vector2 direction = player->attackVelocity.Normalize() * state->showAttackY * 0.5f;

	// キャラを反転させる
	if (direction.x < 0.0f) player->isReverseX = false;
	else if (direction.x > 0.0f) player->isReverseX = true;

	// 手に移動
	Vector2 localPos = Vector2(-12.0f, 45.0f);

	// トランスフォーマー
	Vector2 pos = player->transform.position;
	float rot = atan2f(direction.x, direction.y);
	Vector2 scl = Vector2(state->showAttackX, state->showAttackY);

	// 反転した絵に合わせて位置も反転
	if (player->isReverseX) localPos.x *= -1.0f;

	// 描画する
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


	// ワープ
	if (Input::GetKeyDown(0, Input::LThumb)) {
		// アニメーションの指定
		SetPlayerAnimMove(player->animType);

		// 移動
		player->transform.position = player->warpVelocity;

		// 初期化
		power = 0.0f;
	}
	// ワープ距離のチャージ
	else if (stick != Vector2::Zero) {
		// アニメーションの指定
		SetPlayerAnimMove(player->animType, true);

		// パワー加算
		AddPower(stick);

		// ワープベクトルの指定
		player->warpVelocity = player->transform.position + CalcVector(stick);

		// 重力を徐々になくす
		player->gravityVelocity *= state->friction;
	}
	else {
		// アニメーションの指定
		SetPlayerAnimNoMove(player->animType);

		power = 0.0f;
	}

	player->velocity *= state->friction;
}
void ServerWater::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);

	// 攻撃してない
	if (attack_ == nullptr) {
		// 攻撃
		if (Input::GetKey(0, Input::RThumb)) {
			// アニメーションの指定
			SetPlayerAnimAttack(player->animType);

			// 攻撃オブジェクトの生成
			CreateAttack();
		}
		// チャージ
		else if (stick != Vector2::Zero) {
			// アニメーションの指定
			SetPlayerAnimAttack(player->animType, true);

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
		SetPlayerAnimAttack(player->animType);

		// アタック移動
		attack_->transform.position = player->transform.position;
		attack_->direction = player->attackVelocity * state->atkDistance;

		// 摩擦抵抗
		player->velocity *= state->friction;
	}

	// 攻撃オブジェクトの削除
	if (!Input::GetKey(0, Input::RThumb)) {
		DestroyAttack();

		// アニメーションの指定
		SetPlayerAnimNoAttack(player->animType);
	}
}
AttackServerSide *ServerWater::CreateAttack(void) {
	// 攻撃オブジェクトが生成されているなら終了
	if (attack_) return nullptr;
	// クールタイム中なら終了
	if (state->coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < state->coolTime) return nullptr;



	// 攻撃生成
	attack_ = player->map->GetAttacks()->Add<ServerWaterAttack>(player, this);

	// クールタイム計測開始
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientWater::Move(void) {
	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	float rot = 0.0f;
	Vector2 scl = Vector2(state->showMoveX, state->showMoveY);
	Color col = Color::White;

	// 移動
	if (IsPlayerAnimMove(player->animType)) {
		moveAnim.MoveBegin();
	}
	// チャージ
	else if (IsPlayerAnimMoveCharge(player->animType)) {
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

	moveAnim.Draw(player->transform.position - MultiPlayClient::offset + Vector2(0.0f, state->showMoveY * 0.5f), rot, scl * 2.0f, col);
}
void ClientWater::Attack(void) {
	// 攻撃アニメーションではないなら終了
	if (!IsPlayerAnimAttack(player->animType)) return;




	// 攻撃の向き
	Vector2 direction = player->attackVelocity.Normalize() * state->showAttackY * 0.5f;

	// キャラを反転させる
	if (direction.x < 0.0f) player->isReverseX = false;
	else if (direction.x > 0.0f) player->isReverseX = true;

	// 手に移動
	Vector2 localPos = Vector2(-12.0f, 35.0f);

	// トランスフォーマー
	Vector2 pos = player->transform.position;
	float rot = atan2f(direction.x, direction.y);
	Vector2 scl = Vector2(state->showAttackX, state->showAttackY);

	// 反転した絵に合わせて位置も反転
	if (player->isReverseX) localPos.x *= -1.0f;

	// 描画する
	attackAnim.Draw(pos + direction + localPos - MultiPlayClient::offset, rot, scl, Color::White);
}
void ClientWater::Idle(void) {
	float localScale = 75.0f;


	Vector2 pos = player->transform.position - MultiPlayClient::offset;
	float rot = 0.0f;
	Vector2 scl = Vector2::One * localScale;
	Color col = Color(1.0f,1.0f,1.0f, 0.5f);

	// 待機
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
		// パワー加算
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

	// チャージ中
	if (stick != Vector2::Zero && 0.0f < power) {
		// アニメーションの指定
		SetPlayerAnimMove(player->animType, true);
	}
	// 移動が終わったら（移動終了）
	else if (IsPlayerAnimMove(player->animType) && player->velocity.DistanceSq() < 25.0f) {
		// アニメーションの指定
		SetPlayerAnimNoMove(player->animType);
	}

	// 移動
	if (StickTrigger(stick, previousStick)) {
		// アニメーションの指定
		SetPlayerAnimMove(player->animType);

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

	// チャージ中
	if (stick != Vector2::Zero && 0.0f < power) {
		// アニメーションの指定
		SetPlayerAnimAttack(player->animType, true);
	}
	// 攻撃が終わったら（攻撃終了）
	else if (IsPlayerAnimMove(player->animType) && player->velocity.DistanceSq() < 25.0f) {
		// アニメーションの指定
		SetPlayerAnimNoAttack(player->animType);
	}

	// 攻撃
	if (StickTrigger(stick, previousStick)) {
		// アニメーションの指定
		SetPlayerAnimAttack(player->animType);

		Vector2 direction = (stick - previousStick).Normalize();


		// 攻撃オブジェクトの生成
		auto attack = CreateAttack();

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
	if (state->coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < state->coolTime) return nullptr;



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

	// 回転のスピードを取得
	float rotSpeed = Vector2::Cross(stick, previousStick);

	// 入力
	if (StickTrigger(stick, previousStick)) {
		// パワー加算
		AddPower(rotSpeed);
	}
	else {
		// 回していないときはさらに減速
		power *= state->brakeFriction;
	}




	// パワーの絶対値
	float absPower = MATH::Abs(power);

	// 移動中
	if (state->minActionPower < absPower) {
		// アニメーションの指定
		SetPlayerAnimMove(player->animType);

		// 移動
		player->velocity = absPower * Vector2::Up;

		// 重力をなくす
		player->gravityVelocity = Vector2::Zero;

		// 横移動できないようにする
		horizontalVelocity = 0.0f;
	}
	// 落下
	else if (0.0f > player->gravityVelocity.y) {
		// アニメーションの指定
		SetPlayerAnimNoMove(player->animType);

		horizontalVelocity += stick.x;
		if (maxHorizontalVelocity < horizontalVelocity) horizontalVelocity = maxHorizontalVelocity;
		else if (horizontalVelocity < -maxHorizontalVelocity) horizontalVelocity = -maxHorizontalVelocity;

		// 移動
		player->velocity.x = horizontalVelocity;
	}

	FrictionPower();
	Friction();
}
void ServerWind::Attack(void) {
	using namespace PHYSICS;
	Vector2 stick = Input::GetStickRight(0);
	Vector2 previousStick = Input::GetPreviousStickRight(0);

	// 横移動できないようにする
	horizontalVelocity = 0.0f;

	// 回転のスピードを取得
	float rotSpeed = MATH::Abs(Vector2::Cross(stick, previousStick));

	// 入力
	if (StickTrigger(stick, previousStick)) {
		// パワー加算
		AddPower(rotSpeed);
	}
	else {
		// 回していないときはさらに減速
		power *= state->brakeFriction;
	}




	// パワーの絶対値
	float absPower = MATH::Abs(power);

	// 攻撃中
	if (state->minActionPower < absPower) {
		// アニメーションの指定
		SetPlayerAnimAttack(player->animType);

		// 攻撃オブジェクトの生成
		CreateAttack();

		// アタック移動
		if (attack_) {
			attack_->transform.position = player->transform.position;
		}
	}
	else {
		// アニメーションの指定
		SetPlayerAnimNoAttack(player->animType);

		// 攻撃オブジェクトの削除
		DestroyAttack();
	}

	FrictionPower();
	player->attackVelocity = stick;
}
AttackServerSide *ServerWind::CreateAttack(void) {
	// 攻撃オブジェクトが生成されているなら終了
	if (attack_) return nullptr;
	// クールタイム中なら終了
	if (state->coolTime < 0.0f || coolTimer.GetNowTime() * 0.001f < state->coolTime) return nullptr;



	// 攻撃生成
	attack_ = player->map->GetAttacks()->Add<ServerWindAttack>(player, this);

	// クールタイム計測開始
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientWind::Move(void) {
	// 移動開始
	if (!IsPlayerAnimMove(prevAnimType) && IsPlayerAnimMove(player->animType)) {
		moveAnim.MoveBegin();
		prevPosition = player->transform.position;
	}

	// 現在のアニメーションタイプを記録
	prevAnimType = player->animType;

	// 移動アニメーションではないなら終了
	if (!IsPlayerAnimMove(player->animType)) return;


	Vector2 pos = player->transform.position;
	float rot = 0.0f;
	Vector2 scl = Vector2(state->showMoveX, state->showMoveY);
	Color col = Color::White;
	// 移動
	moveAnim.Draw(prevPosition - MultiPlayClient::offset, rot, scl, col);
}
void ClientWind::Attack(void) {
	// 攻撃アニメーションではないなら終了
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

	// 待機
	if (player->animType == ANIMATION_TYPE_IDLE) {
		idle.Draw(pos + Vector2(0.0f, -70.0f), rot, scl, col);
	}
}

void ServerWindAttack::Loop(void) {

}
void ServerWindAttack::KnockBack(ServerMovableGameObject *object) {
	object->blownVelocity = (object->transform.position - transform.position).Normalize() * knockbackRate;
}

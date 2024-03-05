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
	// 攻撃削除
	if (attack_ && state->atkAfterTime <= atkAfterTimer.GetNowTime() * 0.001f) {
		attack_->Destroy();
		attack_ = nullptr;
		return true;
	}

	return false;
};
void ServerAttribute::LevelUpdate(void) {
	int lv = MAX_LV - 1;
	// レベルが上限の場合
	if (player->lvupPoint[lv] <= player->skillPoint) {
		state = &state_lv[lv];
		this->lv = lv;
	}

	// レベルの調整
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
	// レベルが上限の場合
	if (player->lvupPoint[lv] <= player->skillPoint) {
		state = &state_lv[lv];
	}

	// レベルの調整
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


	// 移動中
	if (StickTrigger(stick)) {
		// アニメーションの指定
		SetPlayerAnimMove(player->animType);

		// パワー加算
		AddPower(stick);

		// ベクトル計算
		velocity += CalcVector(stick) * state->powerMoveRatio;

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
		if (attack_ && IsUseMp()) {
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
	attack_ = player->map->GetAttacks()->Add<ServerFireAttack>(player, this);

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
		if (2 < moveAnims.size()) {
			moveAnims.pop_back();
		}
	}
}
void ClientFire::Attack(void) {
	// 攻撃アニメーションではないなら終了
	if (!IsPlayerAnimAttack(player->animType)) {
		player->isReverseXAttributeControl = false;
		return;
	}


	// レベルによってテクスチャの変更
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

	// 攻撃の向き
	Vector2 direction = player->attackVelocity.Normalize() * state->showAttackY * 0.5f;

	// 属性側でコントロールするか決める
	player->isReverseXAttributeControl = direction != Vector2::Zero;

	// キャラを反転させる
	if (player->isReverseXAttributeControl) {
		if (direction.x < 0.0f) player->isReverseX = false;
		else if (direction.x > 0.0f) player->isReverseX = true;
	}

	// 手に移動
	Vector2 localPos = Vector2(-2.5f, 40.0f);

	// トランスフォーマー
	Vector2 pos = player->transform.position;
	float rot = atan2f(direction.x, direction.y);
	Vector2 scl = Vector2(state->showAttackX, state->showAttackY);

	// 反転した絵に合わせて位置も反転
	if (player->isReverseX) localPos.x *= -1.0f;

	// 描画する
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


	// ワープ
	if (Input::GetKeyDown(0, Input::LThumb)) {
		// アニメーションの指定
		SetPlayerAnimMove(player->animType);

		// 移動
		player->transform.position = player->chargeVelocity;

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
		player->chargeVelocity = player->transform.position + CalcVector(stick) * state->powerMoveRatio;

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
	bool isCharge = IsPlayerAnimAttackCharge(player->animType);
	bool isAttack = IsPlayerAnimAttack(player->animType);

	// 攻撃してない
	if (!isAttack && !isCharge) {
		// 攻撃開始
		if (Input::GetKey(0, Input::RThumb) && IsUseMp()) {
			// アニメーションの指定
			SetPlayerAnimAttack(player->animType, true);

			// 時間計測開始
			attackTimer.Start();
		}
		// チャージ
		else if (stick != Vector2::Zero) {
			// ワープベクトルの指定
			player->attackVelocity = stick.Normalize();

			// パワー加算
			AddPower(stick);
		}
		else {
			// アニメーションの指定
			SetPlayerAnimNoAttack(player->animType);

			// 初期化
			power = 0.0f;
		}
	}

	// ため攻撃
	if (isCharge) {
		// ため攻撃（0.5秒）
		if (0.5f <= attackTimer.GetNowTime() * 0.001f) {

			// 攻撃オブジェクトの生成
			CreateAttack();
		}
	}

	// 攻撃中
	if (attack_) {
		// アニメーションの指定
		SetPlayerAnimAttack(player->animType);

		// アタック移動
		attack_->transform.position = player->transform.position;
		attack_->direction = player->attackVelocity * state->atkDistance;
	}

	if (isAttack || isCharge) {
		// 摩擦抵抗
		player->velocity = Vector2::Zero;
	}

	// 攻撃オブジェクトが削除可能なら
	if (DestroyAttack()) {
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
				player->chargeVelocity - MultiPlayClient::offset,
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
	// インジケーター
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





	// 攻撃の向き
	Vector2 direction = player->attackVelocity.Normalize() * state->showAttackY * 0.5f;

	// レベルによってテクスチャの変更
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

	// 手に移動
	Vector2 localPos = Vector2(-25.0f, 25.0f);

	// トランスフォーマー
	Vector2 pos = player->transform.position;
	float rot = atan2f(direction.x, direction.y);
	Vector2 scl = Vector2(state->showAttackX, state->showAttackY);







	// 攻撃
	if (IsPlayerAnimAttack(player->animType)) {

		// 回転
		player->transform.rotation = rot;

		// キャラを反転させる
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



		// 反転した絵に合わせて位置も反転
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

		// 描画する
		attackAnim.Draw(pos + direction + localPos - MultiPlayClient::offset, rot, scl, Color::White);
		attackSplashAnim.Draw(
			pos + direction.Normalize() * state->showAttackX * 0.08 + localPos - MultiPlayClient::offset,
			rot,
			Vector2::One * state->showAttackX * 0.5f,
			Color::White
		);



		attackChargeAnim.MoveBegin();
	}
	// ため攻撃
	else if (IsPlayerAnimAttackCharge(player->animType)) {

		// 回転
		player->transform.rotation = rot;

		// キャラを反転させる
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


		// 反転した絵に合わせて位置も反転
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



		// 描画する
		rot = atan2f(-direction.y, direction.x);

		// キャラを反転させる
		player->isReverseXAttributeControl = true;
		if (direction.x < 0.0f) player->isReverseX = false;
		else if (direction.x > 0.0f) player->isReverseX = true;

		// 反転した絵に合わせて位置も反転
		if (player->isReverseX) localPos.x *= -1.0f;

		attackChargeAnim.Draw(pos + localPos - MultiPlayClient::offset, rot, Vector2::One * 100.0f, Color::White);
	}
	// その他
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

	// 待機
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
		// パワー加算
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

	// チャージ中
	if (stick != Vector2::Zero && 0.0f < power) {
		// アニメーションの指定
		SetPlayerAnimMove(player->animType, true);

		player->chargeVelocity = -stick;
	}
	// 移動が終わったら（移動終了）
	else if (IsPlayerAnimMove(player->animType) && player->velocity.DistanceSq() < 25.0f) {
		// アニメーションの指定
		SetPlayerAnimNoMove(player->animType);

		// 摩擦抵抗の初期化
		friction = state->friction;
	}
	// 移動中
	else if (IsPlayerAnimMove(player->animType)) {
		// 摩擦抵抗の加算
		friction -= 0.001f;
		if (friction < 0.0f) friction = 0.0f;
	}
	// 何もしていないのにチャージエフェクトになる現象を解消（ゆっくりスティックを戻すことで攻撃トリガーが発動されない）
	if (IsPlayerAnimMoveCharge(player->animType) && stick == Vector2::Zero) {
		// アニメーションの指定
		SetPlayerAnimNoMove(player->animType);
	}

	// 移動
	if (StickTrigger(stick, previousStick)) {
		// アニメーションの指定
		SetPlayerAnimMove(player->animType);

		Vector2 direction = (stick - previousStick).Normalize();
		player->velocity = CalcVector(direction) * state->powerMoveRatio;

		// 初期化
		power = 0.0f;

		// 摩擦抵抗の初期化
		friction = state->friction;
	}
	
	player->velocity *= friction;
}
void ServerThunder::Attack(void) {
	Vector2 stick = Input::GetStickRight(0);
	Vector2 previousStick = Input::GetPreviousStickRight(0);

	// チャージ中
	if (stick != Vector2::Zero && 0.0f < power) {
		// アニメーションの指定
		SetPlayerAnimAttack(player->animType, true);

		player->chargeVelocity = -stick;
	}
	// 攻撃が終わったら（攻撃終了）
	else if (IsPlayerAnimAttack(player->animType)) {
		if (300 < chargeToAttackTimer.GetNowTime()) {
			// アニメーションの指定
			SetPlayerAnimNoAttack(player->animType);
		}
	}
	// 何もしていないのにチャージエフェクトになる現象を解消（ゆっくりスティックを戻すことで攻撃トリガーが発動されない）
	if (IsPlayerAnimAttackCharge(player->animType) && stick==Vector2::Zero) {
		// アニメーションの指定
		SetPlayerAnimNoAttack(player->animType);
	}

	// 攻撃
	if (StickTrigger(stick, previousStick)) {
		// アニメーションの指定
		SetPlayerAnimAttack(player->animType);

		// 時間計測
		chargeToAttackTimer.Start();

		Vector2 direction = (stick - previousStick).Normalize();


		// パワーがあるなら
		if (state->minPower <= power && 0.01f < direction.DistanceSq()) {
			if (IsUseMp()) {

				// 攻撃オブジェクトの生成
				auto attack = CreateAttack();

				// アタック移動
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
					// 攻撃ベクトルの指定
					player->attackVelocity = direction.Normalize();

				}
			}
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
	// レベルによって変更
	if (player->GetLv() < 6) attack_ = player->map->GetAttacks()->Add<ServerThunderAttack>(player, this);
	else attack_ = player->map->GetAttacks()->Add<ServerThunder2Attack>(player, this);
	

	// クールタイム計測開始
	coolTimer.Start();
	atkAfterTimer.Start();

	return attack_;
};

void ClientThunder::Move(void) {
	// インジケーター
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

	// 移動開始
	if (!IsPlayerAnimMove(player->preAnimType) && IsPlayerAnimMove(player->animType)) {
		float distance = player->velocity.Distance();
		Vector2 pos = player->transform.position + player->velocity * 0.5f;
		float rot = std::atan2(player->velocity.y, -player->velocity.x);
		Vector2 scl = Vector2(distance * 7.0f, player->transform.scale.x * 0.75f);

		// 回転
		player->isRotationAttributeControl = true;
		player->transform.rotation = rot;

		// キャラを反転させる
		if (player->velocity.x < 0.0f) {
		}
		else if (player->velocity.x > 0.0f) {
			player->transform.rotation += MATH::Deg2Rad * 180.0f;
		}

		//MultiPlayClient::GetGameMode()->GetMap()->GetEffects()->AddEffect(moveAnim, pos, rot, scl);
	}

	if (IsPlayerAnimMoveCharge(player->preAnimType)) {
		// チャージ描画
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

	// 属性側でコントロールするか決める
	player->isRotationAttributeControl = player->isReverseXAttributeControl = direction != Vector2::Zero;

	// キャラを反転させる
	if (player->isReverseXAttributeControl) {
		float rot = std::atan2(direction.x, direction.y);

		// 回転
		player->transform.rotation = rot;

		// キャラを反転させる
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

	// チャージ中
	if (IsPlayerAnimAttackCharge(player->animType)) {
		Vector2 localPos = direction.Normal().Normalize() * 40.0f;
		Vector2 pos = player->transform.position - MultiPlayClient::offset;
		float rot = atan2f(-direction.y, direction.x);
		Vector2 scl = Vector2::One * 150.0f;
		Color col = Color::White;

		if (player->isReverseX) localPos *= -1.0f;

		// レベルによってテクスチャの変更
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
	// 重力
	velocity += Vector2::Down * gravity;

	// 移動
	transform.position += velocity;

	// 壁に触れたなら削除
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
		player->velocity = absPower * Vector2::Up * state->powerMoveRatio;;

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

		// 減速
		horizontalVelocity *= state->friction;
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
	if (StickTrigger(stick, previousStick) && IsUseMp()) {
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



	// レベルによってテクスチャの変更
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

	// 待機
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

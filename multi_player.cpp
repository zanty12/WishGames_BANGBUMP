#include "multi_player.h"
#include "xinput.h"
#include "sprite.h"
#include "multiplay.h"
#include "multi_skillorb.h"


void ServerPlayer::Loop(void) {
	// ダメージ処理初期化
	damageEffectAttributeType = -1;

	// 落下させる
	gravityVelocity += Vector2::Down * gravity;
	if (-maxGravity >= gravityVelocity.y) gravityVelocity.y = -maxGravity;

	// 吹き飛ばしの速度を減速させる
	blownVelocity *= blownFriction;
	velocity *= friction;

	// 属性使用できるなら
	if (attackAttribute->state->exNoAttributeTime <= exCoolTime.GetNowTime() * 0.001f) {
		// 属性
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

	// 属性切り替えがっちゃんこ！！
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
	// 攻撃者が自分なら終了
	if (attack->GetSelf() == this) return;

	// ダメージエフェクト
	damageEffectAttributeType = attack->GetType();

	// スキルオーブのドロップ
	if (MultiPlayServer::GetGameMode()->GetMode() == MULTI_MODE::FINAL_BATTLE ||
		attack->GetType() == MULTI_OBJECT_TYPE::MULTI_ATTACK_ENEMY2 ||
		attack->GetType() == MULTI_OBJECT_TYPE::MULTI_SPIKE) {
		SkillOrbDrop(attack->atkDrop);
	}

	// ノックバックを与える
	attack->KnockBack(this);
}

void ServerPlayer::SkillOrbDrop(int drop) {
	// マップがないなら終了
	if (!map) return;


	// ドロップする最大値を計算する
	int maxDrop = skillPoint < drop ? skillPoint : drop;

	// スキルオーブをドロップさせる
	map->DropSkillOrb(maxDrop, transform.position, 10.0f);

	skillPoint -= maxDrop;
}



ClientPlayer::ClientPlayer(ATTRIBUTE_TYPE moveAttributeType, ATTRIBUTE_TYPE attackAttributeType, Transform transform) :
	ClientMovableGameObject(transform) {
	// レベル範囲の読み込み
	std::wstring lvStr = L"lv";
	for (int i = 0; i < MAX_LV; i++) {
		lvupPoint[i] = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", lvStr + std::to_wstring(i + 1), 0);
	}


	SetMoveAttribute(moveAttribute);
	SetAttackAttribute(attackAttribute);
	this->transform.scale = Vector2::One * 150;

	preMoveAttributeType = moveAttributeType;
	preAttackAttributeType = attackAttributeType;

	// ダメージエフェクトのロード
	allDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_all.png"), 5, 2, 0, 7, false);
	fireDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_fire.png"), 5, 2, 0, 7, false);
	waterDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_water.png"), 5, 2, 0, 7, false);
	thunderDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_thunder.png"), 5, 2, 0, 7, false);
	windDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_wind.png"), 5, 2, 0, 7, false);
	// その他エフェクト
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
	// 属性がないなら消す
	if (!moveAttribute || !attackAttribute) return;

	// 回転リセット
	if (!isRotationAttributeControl) transform.rotation = 0.0f;



	// がっちゃんこアニメーション開始
	if (preMoveAttributeType == attackAttributeType &&
		preAttackAttributeType == moveAttributeType) {
		exEffect.MoveBegin();
	}

	// アニメーションが開始されていないなら、現在発動している属性をそれぞれ指定する
	if (18 < exEffect.GetIndex()) {
		// 入れ替わってない！？
		if (moveAttribute->GetAttribute() == moveAttributeType && attackAttribute->GetAttribute() == attackAttributeType) {
			curMoveAttribute = moveAttribute, curAttackAttribute = attackAttribute;
			curAnim = &anim;
		}
		// 私たち入れ替わってる！？
		else if (moveAttribute->GetAttribute() == attackAttributeType && attackAttribute->GetAttribute() == moveAttributeType) {
			curMoveAttribute = attackAttribute, curAttackAttribute = moveAttribute;
			curAnim = &reverseAnim;
		}
	}

	if (curAnim) Update(curMoveAttribute, curAttackAttribute, curAnim);



	// アニメーションタイプの更新
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

		// ダメージエフェクト
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

	// アニメーション
	MultiAnimator anim = MultiAnimator(LoadTexture("data/texture/Effect/effect_spawn.png"), 5, 3, 0, 9, false);
	// 落雷を降らす
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

	// レベルを取得
	lv = GetLv();

	// プレイヤー反転
	if (!isReverseXAttributeControl) {
		if (0.0 < velocity.x) isReverseX = true;
		else if (velocity.x < 0.0f) isReverseX = false;
	}

	// 待機アニメーション
	if (moveAttribute) {
		moveAttribute->Idle();
	}


	// ゴメン、メンドクサクナッタ
	// 移動アニメーション（奥表示）
	if (moveAttribute) {
		if (moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_FIRE) {
			// 移動アニメーション
			moveAttribute->LevelUpdate();
			moveAttribute->Move();
		}
	}
	// 攻撃アニメーション（奥前表示）
	if (attackAttribute) {
		if (attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_WIND) {
			// 移動アニメーション
			attackAttribute->LevelUpdate();
			attackAttribute->Attack();
		}
	}


	// アニメーションが切り替わった瞬間、アニメーションする位置を更新する
	if (preAnimType != animType) MultiAnimator::GetPlayer(animType, moveAttribute->GetAttribute(), attackAttribute->GetAttribute(), anim);

	// 描画する
	anim->Draw(transform.position - MultiPlayClient::offset, transform.rotation, transform.scale, Color::White, isReverseX);


	// ゴメン、メンドクサクナッタ　パート２
	// 移動アニメーション（手前表示）
	if (moveAttribute) {
		if (moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_DARK ||
			moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_THUNDER ||
			moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_WIND) {
			// 移動アニメーション
			moveAttribute->LevelUpdate();
			moveAttribute->Move();
		}
	}
	// 攻撃アニメーション（手前表示）
	if (attackAttribute) {
		if (attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_FIRE ||
			attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_DARK ||
			attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_THUNDER) {
			// 移動アニメーション
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
	// nullなら処理しない
	if (moveAttribute == nullptr) return;

	// 削除
	if (this->moveAttribute) delete this->moveAttribute;
	// 設定
	this->moveAttribute = moveAttribute;
	// 属性タイプの設定
	moveAttributeType = preMoveAttributeType = moveAttribute->GetAttribute();
	// アニメーションの設定
	if (attackAttribute) {
		anim = MultiAnimator::GetPlayerInitialize(id % 4, moveAttribute->GetAttribute(), attackAttribute->GetAttribute());
		reverseAnim = MultiAnimator::GetPlayerInitialize(id % 4, attackAttribute->GetAttribute(), moveAttribute->GetAttribute());
	}
}

void ClientPlayer::SetAttackAttribute(ClientAttribute *attackAttribute) {
	// nullなら処理しない
	if (attackAttribute == nullptr) return;

	// 削除
	if (this->attackAttribute) delete this->attackAttribute;
	// 設定
	this->attackAttribute = attackAttribute;
	// 属性タイプの設定
	attackAttributeType = preAttackAttributeType = attackAttribute->GetAttribute();
	// アニメーションの設定
	if (moveAttribute) {
		anim = MultiAnimator::GetPlayerInitialize(id % 4, moveAttribute->GetAttribute(), attackAttribute->GetAttribute());
		reverseAnim = MultiAnimator::GetPlayerInitialize(id % 4, attackAttribute->GetAttribute(), moveAttribute->GetAttribute());
	}
}


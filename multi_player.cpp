#include "multi_player.h"
#include "xinput.h"
#include "sprite.h"
#include "multiplay.h"
#include "multi_skillorb.h"


void ServerPlayer::Loop(void) {
	// 落下させる
	gravityVelocity += Vector2::Down * gravity;
	if (-maxGravity >= gravityVelocity.y) gravityVelocity.y = -maxGravity;

	// 属性
	if (moveAttribute) {
		moveAttribute->LevelUpdate();
		moveAttribute->Move();
	}
	if (attackAttribute) {
		attackAttribute->LevelUpdate();
		attackAttribute->Attack();
	}

	// 属性切り替えがっちゃんこ！！
	if (0.75f < Input::GetTriggerLeft(0) && 0.75f < Input::GetTriggerRight(0)) {
		if (attributeChange == false) {
			ServerAttribute *tmp = attackAttribute;
			attackAttribute = moveAttribute;
			moveAttribute = tmp;
			attributeChange = true;
		}
	}
	else {
		attributeChange = false;
	}

	// 吹き飛ばしの速度を減速させる
	blownVelocity *= blownFriction;
	velocity *= friction;
	if (blownVelocity.DistanceSq() < 1.0f) blownVelocity = Vector2::Zero;
}

void ServerPlayer::Damage(AttackServerSide *attack) {
	// 攻撃者が自分なら終了
	if (attack->GetSelf() == this) return;

	SkillOrbDrop(attack->atkDrop);

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
	SetMoveAttribute(moveAttribute);
	SetAttackAttribute(attackAttribute);
	this->transform.scale = Vector2::One * 150;
}

void ClientPlayer::Loop(void) {
	// 属性がないなら消す
	if (!moveAttribute || !attackAttribute) return;



	// 入れ替わってない！？
	if (moveAttribute->GetAttribute() == moveAttributeType && attackAttribute->GetAttribute() == attackAttributeType) {
		Update(moveAttribute, attackAttribute, &anim);
	}
	// 私たち入れ替わってる！？
	else if (moveAttribute->GetAttribute() == attackAttributeType && attackAttribute->GetAttribute() == moveAttributeType) {
		Update(attackAttribute, moveAttribute, &reverseAnim);
	}


	// アニメーションタイプの更新
	preAnimType = animType;

	// あにめ
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

void ClientPlayer::Update(ClientAttribute *moveAttribute, ClientAttribute *attackAttribute, MultiAnimator *anim) {
	if (timer.GetNowTime() < 200ul && entryType == ENTRY || entryType == NONE) return;

	// 待機アニメーション
	if (moveAttribute) {
		moveAttribute->Idle();
	}


	// ゴメン、メンドクサクナッタ
	// 移動アニメーション（奥表示）
	if (moveAttribute) {
		if (moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_FIRE) {
			// 移動アニメーション
			moveAttribute->Move();
		}
	}
	// 攻撃アニメーション（奥前表示）
	if (attackAttribute) {
		if (attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_WIND) {
			// 移動アニメーション
			attackAttribute->Attack();
		}
	}


	// アニメーションが切り替わった瞬間、アニメーションする位置を更新する
	if (preAnimType != animType) MultiAnimator::GetPlayer(animType, moveAttribute->GetAttribute(), attackAttribute->GetAttribute(), anim);

	// プレイヤー反転
	if (0.0 < velocity.x) isReverseX = true;
	else if (velocity.x < 0.0f) isReverseX = false;

	// 描画する
	anim->Draw(transform.position - MultiPlayClient::offset, transform.rotation, transform.scale, Color::White, isReverseX);


	// ゴメン、メンドクサクナッタ　パート２
	// 移動アニメーション（手前表示）
	if (moveAttribute) {
		if (moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_DARK ||
			moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_THUNDER ||
			moveAttribute->GetAttribute() == ATTRIBUTE_TYPE_WIND) {
			// 移動アニメーション
			moveAttribute->Move();
		}
	}
	// 攻撃アニメーション（手前表示）
	if (attackAttribute) {
		if (attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_FIRE ||
			attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_DARK ||
			attackAttribute->GetAttribute() == ATTRIBUTE_TYPE_THUNDER) {
			// 移動アニメーション
			attackAttribute->Attack();
		}
	}








}

void ClientPlayer::SetMoveAttribute(ClientAttribute *moveAttribute) {
	// nullなら処理しない
	if (moveAttribute == nullptr) return;

	// 削除
	if (this->moveAttribute) delete this->moveAttribute;
	// 設定
	this->moveAttribute = moveAttribute;
	// 属性タイプの設定
	moveAttributeType = moveAttribute->GetAttribute();
	// アニメーションの設定
	if (attackAttribute) {
		anim = MultiAnimator::GetPlayerInitialize(0, moveAttribute->GetAttribute(), attackAttribute->GetAttribute());
		reverseAnim = MultiAnimator::GetPlayerInitialize(0, attackAttribute->GetAttribute(), moveAttribute->GetAttribute());
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
	attackAttributeType = attackAttribute->GetAttribute();
	// アニメーションの設定
	if (moveAttribute) {
		anim = MultiAnimator::GetPlayerInitialize(0, moveAttribute->GetAttribute(), attackAttribute->GetAttribute());
		reverseAnim = MultiAnimator::GetPlayerInitialize(0, attackAttribute->GetAttribute(), moveAttribute->GetAttribute());
	}
}


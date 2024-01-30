#include "multi_player.h"
#include "xinput.h"
#include "sprite.h"
#include "multiplay.h"
#include "multi_skillorb.h"

void ServerPlayer::Loop(void) {

	// 落下させる
	gravityVelocity += Vector2::Down * gravity;
	if (maxGravity >= gravityVelocity.y) gravityVelocity.y = maxGravity;

	if (moveAttribute) moveAttribute->Move();
	if (attackAttribute) attackAttribute->Attack();

	// がっちゃんこシステム
	if (0.75f < Input::GetTriggerLeft(0) && 0.75f < Input::GetTriggerRight(0)) {
		ServerAttribute *tmp = attackAttribute;
		attackAttribute = moveAttribute;
		moveAttribute = tmp;
	}

	// 移動させる
	transform.position += velocity + blownVelocity + gravityVelocity;

	// 吹き飛ばしの速度を減速させる
	blownVelocity *= blownFriction;
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
}

void ClientPlayer::Update(ClientAttribute *moveAttribute, ClientAttribute *attackAttribute, MultiAnimator *anim) {
	// 移動アニメーション
	if (animType == ANIMATION_TYPE_MOVE || animType == ANIMATION_TYPE_MOVE_CHARGE) {
		moveAttribute->Move();
	}
	// 攻撃アニメーション
	if (animType == ANIMATION_TYPE_ATTACK || animType == ANIMATION_TYPE_ATTACK_CHARGE) {
		attackAttribute->Attack();
	}

	// アニメーションが切り替わった瞬間、アニメーションする位置を更新する
	if (preAnimType != animType) MultiAnimator::GetPlayer(animType, moveAttribute->GetAttribute(), attackAttribute->GetAttribute(), anim);

	if (0.0 < velocity.x) isReverseX = true;
	else if (velocity.x < 0.0f) isReverseX = false;


	// 描画する
	anim->Draw(transform.position - MultiPlayClient::offset, transform.rotation, transform.scale, Color::White, isReverseX);
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


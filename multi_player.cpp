#include "multi_player.h"
#include "xinput.h"
#include "sprite.h"
#include "multiplay.h"
#include "multi_skillorb.h"

void ServerPlayer::Loop(void) {
	if (moveAttribute) moveAttribute->Move();
	if (attackAttribute) attackAttribute->Attack();



	transform.position += velocity;
}

void ServerPlayer::Damage(AttackServerSide *attack) {
	// 攻撃者が自分なら終了
	if (attack->GetSelf() == this) return;

	SkillOrbDrop(attack->drop);
}

void ServerPlayer::SkillOrbDrop(int drop) {
	// マップがないなら終了
	if (!map) return;

	while (0 < drop) {
		// ドロップする値
		int tmpDrop = 0;

		// スキルポイントをドロップ（大）
		if (skillPoint >= ServerSkillOrbBig::AddPoint) {
			map->GetSkillOrbs()->Add<ServerSkillOrbBig>(Transform(transform.position));
			tmpDrop = ServerSkillOrbBig::AddPoint;
		}
		// スキルポイントをドロップ（中）
		else if (skillPoint >= ServerSkillOrbMidium::AddPoint) {
			map->GetSkillOrbs()->Add<ServerSkillOrbMidium>(Transform(transform.position));
			tmpDrop = ServerSkillOrbMidium::AddPoint;
		}
		// スキルポイントをドロップ（小）
		else if (skillPoint >= ServerSkillOrbSmall::AddPoint) {
			map->GetSkillOrbs()->Add<ServerSkillOrbSmall>(Transform(transform.position));
			tmpDrop = ServerSkillOrbSmall::AddPoint;
		}
		else return;

		// スキルポイントを減らす
		skillPoint -= tmpDrop;
		drop -= tmpDrop;

		// 最低値0にする
		if (skillPoint < 0) skillPoint = 0;
	}

}



ClientPlayer::ClientPlayer(ATTRIBUTE_TYPE moveAttributeType, ATTRIBUTE_TYPE attackAttributeType, Transform transform) :
	moveAttribute(ClientAttribute::Create(this, moveAttributeType)), 
	attackAttribute(ClientAttribute::Create(this, attackAttributeType)) ,
	ClientMovableGameObject(transform) {	
	anim = MultiAnimator::GetPlayerInitialize(0, moveAttribute->GetAttribute(), attackAttribute->GetAttribute());
	this->transform.scale = Vector2::One * 150;
}

void ClientPlayer::Loop(void) {
	// 属性がないなら消す
	if (!moveAttribute || !attackAttribute) return;


	// 移動アニメーション
	if (animType == ANIMATION_TYPE_MOVE || animType == ANIMATION_TYPE_MOVE_CHARGE) {
		moveAttribute->Move();
	}
	// 攻撃アニメーション
	if (animType == ANIMATION_TYPE_ATTACK || animType == ANIMATION_TYPE_ATTACK_CHARGE) {
		attackAttribute->Attack();
	}

	// アニメーションが切り替わった瞬間、アニメーションする位置を更新する
	if (preAnimType != animType) MultiAnimator::GetPlayer(animType, moveAttribute->GetAttribute(), attackAttribute->GetAttribute(), &anim);
	
	if (0.0 < velocity.x) isReverseX = true;
	else if (velocity.x < 0.0f) isReverseX = false;

	// 描画する
	anim.Draw(transform.position - MultiPlayClient::offset, transform.rotation, transform.scale, Color::White, isReverseX);

	// アニメーションタイプの更新
	preAnimType = animType;
}


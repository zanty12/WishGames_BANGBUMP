#pragma once
#include <sstream>
#include "multi_movable_object.h"
#include "multi_attribute.h"
#include "multi_anim.h"
#include "multi_map.h"
#include "attribute_type.h"
#include "multi_animenum.h"

/*******************************************************
  Server
********************************************************/
class ServerAttribute;
class AttackServerSide;
class ServerPlayer : public ServerMovableGameObject {
private:
	ServerAttribute *moveAttribute = nullptr;			// 移動属性
	ServerAttribute *attackAttribute = nullptr;			// 攻撃属性


public:
	int skillPoint = 0;									// スキルポイント
	int score = 0;										// スコア
	MultiMap *map = nullptr;							// マップ
	MULTI_ANIMATION_TYPE animType = ANIMATION_TYPE_IDEL;// アニメーション
	Vector2 attackVelocity;								// 攻撃のベロシティ
	Vector2 warpVelocity;								// ワープベロシティ
	Vector2 blownVelocity;								// ダメージ時吹き飛ばされたベロシティ
	float blownFriction = 0.85f;						// ダメージ時吹き飛ばされた際の減速度
	
public:
	ServerPlayer() { gravity = 0.5f; }
	~ServerPlayer() {
		if (moveAttribute) delete moveAttribute;
		if (attackAttribute) delete attackAttribute;
		moveAttribute = nullptr;
		attackAttribute = nullptr;
	}
	void Damage(AttackServerSide *attack) override;
	void SkillOrbDrop(int drop);

	void SetMoveAttribute(ServerAttribute *moveAttribute) { this->moveAttribute = moveAttribute; }
	void SetAttackAttribute(ServerAttribute *attackAttribute) { this->attackAttribute = attackAttribute; }
	void SetAttribute(ServerAttribute *moveAttribute, ServerAttribute *attackAttribute) { this->moveAttribute = moveAttribute, this->attackAttribute = attackAttribute; }
	ServerAttribute *GetMoveAttribute(void) { return moveAttribute; }
	ServerAttribute *GetAttackAttribute(void) { return attackAttribute; }

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_PLAYER; }
};






/*******************************************************
  Client
********************************************************/
class ClientAttribute;
class ClientAttack;
class ClientPlayer : public ClientMovableGameObject {
private:
	ClientAttribute *moveAttribute = nullptr;			// 移動属性
	ClientAttribute *attackAttribute = nullptr;			// 攻撃属性
	MultiAnimator reverseAnim;							// 反転アニメーション

public:
	int skillPoint = 0;										// スキルポイント
	MULTI_ANIMATION_TYPE animType = ANIMATION_TYPE_IDEL;	// アニメーションタイプ
	MULTI_ANIMATION_TYPE preAnimType = ANIMATION_TYPE_IDEL;	// アニメーション（1フレーム前）
	ATTRIBUTE_TYPE moveAttributeType;					// 移動属性タイプ
	ATTRIBUTE_TYPE attackAttributeType;					// 攻撃属性タイプ
	MultiAnimator anim;										// アニメーション
	bool isReverseX = false;								// 横軸の向き
	Vector2 attackVelocity;									// 攻撃のベロシティ
	Vector2 warpVelocity;									// ワープベロシティ


	
public:
	ClientPlayer(ATTRIBUTE_TYPE moveAttributeType, ATTRIBUTE_TYPE attackAttributeType, Transform transform);

	void Loop(void) override;

	void Update(ClientAttribute *moveAttribute, ClientAttribute *attackAttribute, MultiAnimator *anim);
	void SetMoveAttribute(ClientAttribute *moveAttribute);
	void SetAttackAttribute(ClientAttribute *attackAttribute);
	void SetAttribute(ClientAttribute *moveAttribute, ClientAttribute *attackAttribute) {
		SetMoveAttribute(moveAttribute);
		SetAttackAttribute(attackAttribute);
	}
	ClientAttribute *GetMoveAttribute(void) { return moveAttribute; }
	ClientAttribute *GetAttackAttribute(void) { return attackAttribute; }
};

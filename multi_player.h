#pragma once
#include <sstream>
#include "lib/win_time.h"
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
	MULTI_ANIMATION_TYPE animType = ANIMATION_TYPE_IDLE;// アニメーション
	Vector2 attackVelocity;								// 攻撃のベクトル
	Vector2 warpVelocity;								// ワープベクトル
	bool attributeChange = false;						// 属性チェンジ


public:
	ServerPlayer() { gravity = 0.01f; }
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
class AttackClientSide;
class ClientPlayer : public ClientMovableGameObject {
private:
	enum ENTRY_TYPE {
		NONE,
		ENTRY,
		SHOW,
		EXIT
	};

private:
	ClientAttribute *moveAttribute = nullptr;				// 移動属性
	ClientAttribute *attackAttribute = nullptr;				// 攻撃属性
	MultiAnimator reverseAnim;								// 反転アニメーション
	WIN::Time timer;										// スポーン開始時からの時間計測
	ENTRY_TYPE entryType = NONE;							// 入場演出

public:
	int skillPoint = 0;										// スキルポイント
	MULTI_ANIMATION_TYPE animType = ANIMATION_TYPE_IDLE;	// アニメーションタイプ
	MULTI_ANIMATION_TYPE preAnimType = ANIMATION_TYPE_IDLE;	// アニメーション（1フレーム前）
	ATTRIBUTE_TYPE moveAttributeType;						// 移動属性タイプ
	ATTRIBUTE_TYPE attackAttributeType;						// 攻撃属性タイプ
	MultiAnimator anim;										// アニメーション
	bool isReverseX = false;								// 横軸の向き
	Vector2 attackVelocity;									// 攻撃のベロシティ
	Vector2 warpVelocity;									// ワープベロシティ


	
public:
	ClientPlayer(ATTRIBUTE_TYPE moveAttributeType, ATTRIBUTE_TYPE attackAttributeType, Transform transform);

	void Loop(void) override;
	void ShowEntry();
	void ShowExit();

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

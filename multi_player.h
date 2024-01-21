#pragma once
#include <sstream>
#include "attribute_type.h"
#include "multi_animenum.h"
#include "multi_object.h"
#include "multi_map.h"
#include "multi_attribute.h"
#include "multi_anim.h"

class MultiAttribute;
class ServerPlayer : public ServerGameObject {
private:
	MultiAttribute *moveAttribute = nullptr;			// 移動属性
	MultiAttribute *attackAttribute = nullptr;			// 攻撃属性


public:
	int skillPoint = 0;									// スキルポイント
	MultiMap *map = nullptr;							// マップ
	ANIMATION_TYPE animType = ANIMATION_TYPE_IDEL;		// アニメーション
	
	
public:
	~ServerPlayer() {
		if (moveAttribute) delete moveAttribute;
		if (attackAttribute) delete attackAttribute;
		moveAttribute = nullptr;
		attackAttribute = nullptr;
	}

	void SetMoveAttribute(MultiAttribute *moveAttribute) { this->moveAttribute = moveAttribute; }
	void SetAttackAttribute(MultiAttribute *attackAttribute) { this->attackAttribute = attackAttribute; }
	void SetAttackAttribute(MultiAttribute *moveAttribute, MultiAttribute *attackAttribute) { this->moveAttribute = moveAttribute, this->attackAttribute = attackAttribute; }
	MultiAttribute *GetMoveAttribute(void) { return moveAttribute; }
	MultiAttribute *GetAttackAttribute(void) { return attackAttribute; }

	void Loop(void) override;
};

class ClientPlayer : public ClientGameObject {
private:
	ANIMATION_TYPE preAnimType = ANIMATION_TYPE_IDEL;	// アニメーション（1フレーム前）

public:
	int skillPoint = 0;										// スキルポイント
	ATTRIBUTE_TYPE moveAttribute = ATTRIBUTE_TYPE_FIRE;		// 移動属性
	ATTRIBUTE_TYPE attackAttribute = ATTRIBUTE_TYPE_FIRE;	// 攻撃属性
	ANIMATION_TYPE animType = ANIMATION_TYPE_IDEL;			// アニメーション
	MultiAnimator anim;
	bool isReverseX = false;								// 横軸の向き


	
public:
	ClientPlayer(ATTRIBUTE_TYPE moveAttribute, ATTRIBUTE_TYPE attackAttribute, Transform transform)
		: moveAttribute(moveAttribute), attackAttribute(attackAttribute), ClientGameObject(transform) {
		anim = MultiAnimator::GetPlayerInitialize(0, moveAttribute, attackAttribute);
	}

	void Loop(void) override;

};
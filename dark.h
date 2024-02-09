//--------------------------------------------------------------------------------
// 
// ワープ[fark.h]
// 
// 作成者 趙潤博
// 
// 最終更新日	2023/11/22
// 
//--------------------------------------------------------------------------------
#pragma once
#include "attribute.h"
#include "thunder.h"

class DarkAttack;
class DarkIndicator;
class DarkAttackIndicator;
class DarkEffect;
class Dark : public Attribute
{
	Vector2 warpPosition;
	Vector2 attackDirection_;						// 攻撃する向き
	//const float maxSpeedFalling = 0.5f;				// 落下中のスピード
	//const float warpDistance = 800.0f;				// ワープ距離
	const float responseMinStickDistance = 0.2f;	// スティックの傾けたときに判定する最小値
	DarkAttack* attack_ = nullptr;
	DarkIndicator* move_indicator_ = nullptr;
	DarkAttackIndicator* attack_indicator_ = nullptr;
	DarkEffect* move_effect_ = nullptr;
	//ここからは調整用のためconst抜き
	float maxSpeedFalling = -0.5f;
	float warpDistance_ = 20*GameObject::SIZE_;

public:

	Dark(Player* player);
	~Dark() override {
		if (attack_) delete attack_;
		if (move_indicator_) delete move_indicator_;
		if (attack_indicator_) delete attack_indicator_;
		if (move_effect_) delete move_effect_;
	}
	Vector2 Move() override;
	void Action() override;
	void DebugMenu() override;

	void Gatchanko(bool is_attack) override;
};

class DarkAttack : public MovableObj,public PlayerAttack
{
	Dark* parent_;
	Vector2 size_ = Vector2(2* GameObject::SIZE_, 5*GameObject::SIZE_);
public:
	DarkAttack() = delete;
	DarkAttack(Dark* parent);
	~DarkAttack() override = default;
	void Update() override;
};

class DarkIndicator : public MovableObj
{
private:
	Vector2 target_pos_;
public:
	DarkIndicator();
	~DarkIndicator() override = default;
	void Update() override;
	void SetTargetPos(Vector2 pos) { target_pos_ = pos; }
};

class DarkAttackIndicator : public MovableObj
{
public:
	DarkAttackIndicator();
	~DarkAttackIndicator() override = default;
	void Update() override {};
};

class DarkEffect : public MovableObj
{
	Dark* parent_;
	float move_time_;
	bool teleport_;	//テレポート
	bool charge_;	//チャージ

public:
	DarkEffect() = delete;
	DarkEffect(Dark* parent);
	~DarkEffect() override = default;
	void Update() override;
	void Idle();
	void Move();
	void Charge();
};

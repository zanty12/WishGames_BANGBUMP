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
class Dark : public Attribute
{
	Vector2 warpPosition;
	Vector2 attackDirection_;						// 攻撃する向き
	//const float maxSpeedFalling = 0.5f;				// 落下中のスピード
	//const float warpDistance = 800.0f;				// ワープ距離
	const float responseMinStickDistance = 0.2f;	// スティックの傾けたときに判定する最小値
	DarkAttack* attack_ = nullptr;
	DarkIndicator* move_indicator_ = nullptr;
	ThunderIndicator* attack_indicator_ = nullptr;
	//ここからは調整用のためconst抜き
	float maxSpeedFalling = -0.5f;
	float warpDistance_ = 20*GameObject::SIZE_;

public:

	Dark(Player* player) : Attribute(player, ATTRIBUTE_TYPE_DARK) {}
	~Dark() override = default;
	Vector2 Move() override;
	void Action() override;
	void DebugMenu() override;
};

class DarkAttack : public MovableObj
{
	Dark* parent_;
	Vector2 size_ = Vector2(5* GameObject::SIZE_, 2*GameObject::SIZE_);
public:
	DarkAttack() = delete;
	DarkAttack(Dark* parent);
	~DarkAttack() override = default;
	void Update() override{};
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
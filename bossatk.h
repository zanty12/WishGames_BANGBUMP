#pragma once
#include "enemy.h"
#include "movableobj.h"
#include "sprite.h"
#include "animator.h"
#include "time.h"



class Boss_Fire : public MovableObj
{
private:
	bool cheak_;
	float range_;
	float time_;
	float speed_;
	Vector2 boss_pos_;

public:
	Boss_Fire() = delete;
	~Boss_Fire() override = default;

	Boss_Fire(Vector2 pos);
	void Update() override;
};
class Boss_Thunder : public MovableObj
{
private:
	float time_;
	float speed_;
	Vector2 boss_pos_;

public:
	Boss_Thunder() = delete;
	~Boss_Thunder() override = default;

	Boss_Thunder(Vector2 pos);
	void Update() override;


};
class Boss_Wind : public MovableObj
{
private:
	float time_;
	float speed_;
	Vector2 boss_pos_;
	Vector2 Sp, Ep;			// 線の始点と終点
	Vector2 N;				// 法線ベクトル
	Vector2 vel_;

public:
	Boss_Wind() = delete;
	~Boss_Wind() override = default;

	Boss_Wind(Vector2 pos, float spx, float spy, float epx, float epy);
	void Update() override;

};
class Boss_Water : public MovableObj
{
private:
	float time_;
	Vector2 boss_pos_;

public:
	Boss_Water() = delete;
	~Boss_Water() override = default;

	Boss_Water(Vector2 pos);
	void Update() override;
};



#pragma once
#include "enemy.h"
#include "movableobj.h"
#include "sprite.h"
#include "animator.h"
#include "time.h"


class Boss_Fire : public MovableObj
{
private:
	float time_;
	bool cheak_;
	float range_;
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


public:
	Boss_Thunder() = delete;
	~Boss_Thunder() override = default;

	Boss_Thunder(Vector2 pos);
	void Update() override;
};
class Boss_Wind : public MovableObj
{
private:


public:
	Boss_Wind() = delete;
	~Boss_Wind() override = default;

	Boss_Wind(Vector2 pos);
	void Update() override;
};
class Boss_Water : public MovableObj
{
private:


public:
	Boss_Water() = delete;
	~Boss_Water() override = default;

	Boss_Water(Vector2 pos);
	void Update() override;
};



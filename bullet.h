/*==============================================================================
   バレット管理[bullet.h]
==============================================================================*/
#pragma once
#include "movableobj.h"
#include "sprite.h"
#include "animator.h"

class Bullet : public MovableObj
{
private:
	int spd_;
	int atk_;
	bool dead = false; //死んでいるかどうか

public:

	Bullet() = delete;
	~Bullet() override = default;

	int GetAtk(void) const { return atk_; }
	void SetAtk(int atk) { atk_ = atk; }

	Bullet(Vector2 pos);

	//Bullet(Vector2 pos, Vector2 vel);

	void Update() override;

	int GetSpd(void) { return spd_; }		//HPのゲット
	void SetSpd(int spd) { spd_ = spd; }			//HPのセット

	void Die() { this->dead = true; }

private:

	void CollisionAction(void);

};
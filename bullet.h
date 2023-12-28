/*==============================================================================
   ÉoÉåÉbÉgä«óù[bullet.h]
==============================================================================*/
#pragma once
#include "movableobj.h"
#include "sprite.h"
#include "animator.h"

class Bullet : public MovableObj
{
private:

	const float spd_ = 96.0f * 2;
	int atk_ = 10;


public:

	Bullet() = delete;
	~Bullet() override = default;

	int GetAtk(void) const { return atk_; }
	void SetAtk(int atk) { atk_ = atk; }

	Bullet(Vector2 pos, int texNo, Vector2 vel) : MovableObj(pos, 0.0f, texNo, vel)
	{
	
	}

	Bullet(Vector2 pos, Vector2 vel);

	void Update() override;


};
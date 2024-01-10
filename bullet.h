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
	Vector2 startPosition;
	int atk_;

public:

	Bullet() = delete;
	~Bullet() override = default;

	int GetAtk(void) const { return atk_; }
	void SetAtk(int atk) { atk_ = atk; }

	Bullet(Vector2 pos);


	void Update() override;


	void Die() { Discard(); }

private:

	void CollisionAction(void);

};
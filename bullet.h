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

	Bullet(int x, int y, int texNo) : MovableObj(Vector2(x * SIZE_, y * SIZE_), 0.0f, texNo, Vector2(0.0f, 0.0f)) {}

	Bullet(int x, int y) : Bullet(x, y, LoadTexture("data/texture/bullet.png")) {}
	
	void Update() override;


};
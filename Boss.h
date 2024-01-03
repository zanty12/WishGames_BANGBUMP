#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"
#include "time.h"

enum BOSSATK_ATTRIBUTE
{
	ATTRIBUTE_FIRE,
	ATTRIBUTE_THUNDER,
	ATTRIBUTE_WIND,
	ATTRIBUTE_WATER,
};


class Boss : public Enemy
{
private:

	Vector2 startPosition;
	float rangeDegree;		// ”ÍˆÍ(Šp“x)
	float length;			// ’·‚³
	float directionDegree;	// •ûŒü(Šp“x)

	BOSSATK_ATTRIBUTE boss_attribute_;
	SKILLORB_ATTRIBUTE_DESC drop;

public:

	Boss() = delete;
	~Boss() override = default;

	Boss(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y, LoadTexture("data/texture/ordin.png"), enemy_mngr)
	{
		startPosition = GetPos();
		SetScale(Vector2(SIZE_ * 5, SIZE_ * 7));
		SetHp(300);
	}

	void Update() override;
	
	SkillOrb* DropSkillOrb() override;
	void Atk();
	BOSSATK_ATTRIBUTE GetBoosAttribute() const { return boss_attribute_; }

	void Fire();
	void Thunder();
	void Wind();
	void Water();

};

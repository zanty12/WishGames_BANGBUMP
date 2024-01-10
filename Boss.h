#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"
#include "time.h"


class Boss : public Enemy
{
private:

	Vector2 startPosition;
	float rangeDegree;		// ”ÍˆÍ(Šp“x)
	float length;			// ’·‚³
	float directionDegree;	// •ûŒü(Šp“x)
	float time_;			// ŠÔ
	float atk_time_;		// UŒ‚ŠÔ
	bool atk_now;			// UŒ‚ƒ`ƒFƒbƒN

	//SKILLORB_ATTRIBUTE_DESC drop;

public:

	Boss() = delete;
	~Boss() override = default;

	Boss(int x, int y, EnemyMngr* enemy_mngr);

	void Update() override;

	SkillOrb* DropSkillOrb();

	void Atk();

	void Fire();
	void Thunder();
	void Wind();
	void Water();

};

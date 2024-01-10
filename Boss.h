#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"
#include "time.h"


class Boss : public Enemy
{
private:

	Vector2 startPosition;
	float rangeDegree;		// �͈�(�p�x)
	float length;			// ����
	float directionDegree;	// ����(�p�x)
	float time_;			// ����
	float atk_time_;		// �U������
	bool atk_now;			// �U���`�F�b�N

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

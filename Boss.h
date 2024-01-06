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
	float rangeDegree;		// �͈�(�p�x)
	float length;			// ����
	float directionDegree;	// ����(�p�x)
	float time_;			// ����
	float atk_time_;		// �U������
	bool atk_now;			// �U���`�F�b�N

	BOSSATK_ATTRIBUTE boss_attribute_;
	SKILLORB_ATTRIBUTE_DESC drop;

public:

	Boss() = delete;
	~Boss() override = default;

	Boss(int x, int y, EnemyMngr* enemy_mngr) : Enemy(x, y, LoadTexture("data/texture/boss.png"), enemy_mngr)
	{
		startPosition = GetPos();
		SetScale(Vector2(SIZE_ * 6, SIZE_ * 6));
		SetHp(300);
		time_ = 0;
		atk_time_ = 0;
		atk_now = false;
	}

	void Update() override;

	SkillOrb* DropSkillOrb();

	void Atk();
	BOSSATK_ATTRIBUTE GetBoosAttribute() const { return boss_attribute_; }

	void Fire();
	void Thunder();
	void Wind();
	void Water();

};

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
	MAP_READ GetCellTypeWall() const { return MAP_READ_WALL; }

};

class Reflection
{
private:

	Vector2 Sp, Ep;			// ���̎n�_�ƏI�_
	Vector2 N;				// �@���x�N�g��
	Vector2 vel_;

public:

	Reflection(float spx, float spy, float epx, float epy)
	{
		Sp.x = spx;
		Sp.y = spy;
		Ep.x = epx;
		Ep.y = epy;
		Vector2 v = Ep - Sp;

		

		N.x = -v.y;
		N.y = v.x;
		N.Normalize();

	}


};

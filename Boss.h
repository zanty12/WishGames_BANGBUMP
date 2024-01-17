#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"
#include "time.h"

class Boss : public Enemy
{
private:

	Vector2 startPosition;
	float rangeDegree;		// 範囲(角度)
	float length;			// 長さ
	float directionDegree;	// 方向(角度)
	float time_;			// 時間
	float atk_time_;		// 攻撃時間
	bool atk_now;			// 攻撃チェック

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

	Vector2 Sp, Ep;			// 線の始点と終点
	Vector2 N;				// 法線ベクトル
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

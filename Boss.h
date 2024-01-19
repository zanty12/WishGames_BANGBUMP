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
	bool change_scene_;		//シーン遷移フラグ

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

	bool GetChangeSceneFlag(void) const { return change_scene_; }	//シーンチェンジのフラグ true=別のシーンへ

};


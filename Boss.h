#pragma once
#include "enemy.h"
#include "sprite.h"
#include "animator.h"
#include "time.h"

class Boss_Thunder;

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

	bool thunder_on_;		// �T���_�[�A�^�b�N�t���O
	Boss_Thunder* atk_thunder_ = nullptr;	//�T���_�[�A�^�b�N
	int thunder_num_;
	bool thunder_die_ = false;	//�T���_�[���������Ƃ�

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


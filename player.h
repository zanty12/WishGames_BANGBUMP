//--------------------------------------------------------------------------------
// 
// �v���C���[[player.h]
// 
// �쐬�� ���{��
// 
// �쐬��		2023/11/16
// �ŏI�X�V��	2023/11/20
// 
//--------------------------------------------------------------------------------

#pragma once

#include "lib/dxlib.h"

#include "sprite.h"
#include "movableobj.h"
#include "attribute.h"

class Player : public MovableObj
{
private:
	const int SKILL_GAUGE_MAX_ = 10;	//�����X�L���|�C���g�̏��
	const int HP_MAX_ = 1000;			//HP�̏��
	const float GRAVITY_SCALE_ = 6.0f;	//�d�́i���j

	Vector2 dir_;		//����
	Vector2 scale_;		//�傫���i����j
	Color color_;

	class Attribute* attribute_ = nullptr;
	int hp_;
	int skillpt_;

public:
	Player(Vector2 pos,float rot, int tex_number,Vector2 vel)
		:MovableObj(pos,rot,tex_number,vel),hp_(HP_MAX_),skillpt_(0),
		dir_(Vector2(0.0f,0.0f)), scale_(Vector2(100.0f,100.0f/*����Ƃ肠����100*/)),color_(Color(1.0f, 1.0f, 1.0f, 1.0f)) {}

	void SetDir(Vector2 dir) { dir_ = dir; }	//�����̃Z�b�g
	Vector2 GetDir(void) const { return dir_; }	//�����̃Q�b�g
	int GetHp(void) const { return hp_; }		//HP�̃Q�b�g
	void SetAttribute(Attribute* attribute) { attribute_ = attribute; }	//�A�g���r���[�g�|�C���^�̃Z�b�g�i�������삵�Ă��Ȃ��Ƃ���nullptr���Z�b�g�j

	//�X�L���|�C���g�̎g�p�i�g����Ƃ�=true �g���ƃX�L���|�C���g��0�ɂȂ�j
	bool UseSkillPoint(void);
	//�X�L���|�C���g�̑����i�Q�b�g�����|�C���g�Ə����X�L���|�C���g�̍��v�������X�L���|�C���g�̏���𒴂���ꍇ�A�����X�L���|�C���g��10�ɂȂ�j
	void SkillPointUp(int point) { skillpt_ + point <= SKILL_GAUGE_MAX_ ? skillpt_ += point : skillpt_ = SKILL_GAUGE_MAX_; }
	//�X�L���|�C���g�̌����i�_���[�W�������X�L���|�C���g�𒴂���ꍇ�A�X�L���|�C���g��0�ɂȂ�j
	void SkillPointDown(int damage) { damage <= skillpt_ ? skillpt_ -= damage : skillpt_ = 0; }
	//HP�̌����i�_���[�W�����݂�HP�𒴂���ꍇ�AHP��0�ɂȂ�j
	void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

	void Update(void) override;
	void Draw(void) override { DrawSprite(GetTexNo(), GetPos(), GetRot(), scale_, color_); }

private:
	//�����̃A�b�v�f�[�g�B���x�����ƂɍX�V�i�S�������Ă��Ȃ��ꍇ�͎~�܂����u�Ԃ̌����̂܂܂ɂ���j
	void UpdateDir(void) { if (GetVel() != Vector2(0.0f, 0.0f)) dir_ = GetVel().Normalize(); }

};

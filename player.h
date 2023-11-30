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
#include "mapmngr.h"

class MapMngr;
class Player : public MovableObj
{
private:
	const int SKILL_GAUGE_MAX_ = 10;	//�����X�L���|�C���g�̏��
	const int HP_MAX_ = 1000;			//HP�̏��
	const float GRAVITY_SCALE_ = 6.0f;	//�d�́i���j

	Vector2 dir_;		//����

	class Attribute* move_attribute_ = nullptr;		//�����p�̃A�g���r���[�g
	class Attribute* attack_attribute_ = nullptr;	//�U���p�̃A�g���r���[�g

	int hp_;
	int skillpt_;

	MapMngr* map_mangr_;

public:
	Player(Vector2 pos,float rot, int tex_number,Vector2 vel , MapMngr* map_mangr)
		:MovableObj(pos,rot,tex_number,vel),hp_(HP_MAX_),skillpt_(0),
		dir_(Vector2(0.0f,0.0f)),map_mangr_(map_mangr) {}

	void SetDir(Vector2 dir) { dir_ = dir; }	//�����̃Z�b�g
	Vector2 GetDir(void) const { return dir_; }	//�����̃Q�b�g
	int GetHp(void) const { return hp_; }		//HP�̃Q�b�g
	void SetAttribute(Attribute* move_attribute) {delete move_attribute_; move_attribute_ = move_attribute; }				//���[�u�A�g���r���[�g�|�C���^�̃Z�b�g�i�������삵�Ă��Ȃ��Ƃ���nullptr���Z�b�g�j
	void SetAttackAttribute(Attribute* attack_attribute) {delete attack_attribute_; attack_attribute_ = attack_attribute; }	//�A�^�b�N�A�g���r���[�g�|�C���^�̃Z�b�g�i�������삵�Ă��Ȃ��Ƃ���nullptr���Z�b�g�j
	Attribute* GetAttribute(void) { return move_attribute_; }			//���[�u�A�g���r���[�g�|�C���^���Q�b�g�i�����������Ȃ����nullptr��Ԃ��j
	Attribute* GetAttackAttribute(void) { return attack_attribute_; }	//�A�^�b�N�A�g���r���[�g�|�C���^���Q�b�g�i�����������Ȃ����nullptr��Ԃ��j
	MapMngr* GetMapMngr(void) { return map_mangr_; }	//MapMngr�̃|�C���^���Q�b�g

	//�X�L���|�C���g�̎g�p�i�g����Ƃ�=true �g���ƃX�L���|�C���g��0�ɂȂ�j
	bool UseSkillPoint(void);
	//�X�L���|�C���g�̑����i�Q�b�g�����|�C���g�Ə����X�L���|�C���g�̍��v�������X�L���|�C���g�̏���𒴂���ꍇ�A�����X�L���|�C���g��10�ɂȂ�j
	void SkillPointUp(int point) { skillpt_ + point <= SKILL_GAUGE_MAX_ ? skillpt_ += point : skillpt_ = SKILL_GAUGE_MAX_; }
	//�X�L���|�C���g�̌����i�_���[�W�������X�L���|�C���g�𒴂���ꍇ�A�X�L���|�C���g��0�ɂȂ�j
	void SkillPointDown(int damage) { damage <= skillpt_ ? skillpt_ -= damage : skillpt_ = 0; }
	//HP�̌����i�_���[�W�����݂�HP�𒴂���ꍇ�AHP��0�ɂȂ�j
	void HpDown(int damage) { damage <= hp_ ? hp_ -= damage : hp_ = 0; }

	void Update(void) override;

private:
	//�����̃A�b�v�f�[�g�B���x�����ƂɍX�V�i�S�������Ă��Ȃ��ꍇ�͎~�܂����u�Ԃ̌����̂܂܂ɂ���j
	void UpdateDir(void) { if (GetVel() != Vector2(0.0f, 0.0f)) dir_ = GetVel().Normalize(); }

	//�����蔻��i�}�b�v�j
	void CollisionMap(void);
};

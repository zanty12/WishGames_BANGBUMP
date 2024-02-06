#pragma once
#include <sstream>
#include "lib/win_time.h"
#include "multi_movable_object.h"
#include "multi_attribute.h"
#include "multi_anim.h"
#include "multi_map.h"
#include "attribute_type.h"
#include "multi_animenum.h"
#include "multi_path.h"
#include "ini.h"

/*******************************************************
  Server
********************************************************/
class ServerAttribute;
class AttackServerSide;
class ServerPlayer : public ServerMovableGameObject {
private:
	ServerAttribute *moveAttribute = nullptr;			// �ړ�����
	ServerAttribute *attackAttribute = nullptr;			// �U������
	WIN::Time exCoolTime;								// ��������񂱃N�[���^�C��


public:
	int skillPoint = 0;									// �X�L���|�C���g
	int score = 0;										// �X�R�A
	int animType = ANIMATION_TYPE_IDLE;					// �A�j���[�V����
	MultiMap *map = nullptr;							// �}�b�v
	Vector2 attackVelocity;								// �U���̃x�N�g��
	Vector2 warpVelocity;								// ���[�v�x�N�g��
	bool isAttributeChange = false;						// �����`�F���W
	WIN::Time exStartTime;								// ��������񂱊J�n�^�C�}�[


public:
	ServerPlayer() { 
		radius = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", L"radius", 10.0f);
		gravity = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", L"gravity", 0.01f);
		maxGravity = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", L"maxGravity", 0.5f);
		transform.scale.x = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", L"width", 20.0f);
		transform.scale.y = ini::GetFloat(PARAM_PATH + L"player.ini", L"Player", L"height", 60.0f);
		exCoolTime.Start();
	}
	~ServerPlayer() {
		if (moveAttribute) delete moveAttribute;
		if (attackAttribute) delete attackAttribute;
		moveAttribute = nullptr;
		attackAttribute = nullptr;
	}
	void Damage(AttackServerSide *attack) override;
	void SkillOrbDrop(int drop);

	void SetMoveAttribute(ServerAttribute *moveAttribute) { this->moveAttribute = moveAttribute; }
	void SetAttackAttribute(ServerAttribute *attackAttribute) { this->attackAttribute = attackAttribute; }
	void SetAttribute(ServerAttribute *moveAttribute, ServerAttribute *attackAttribute) { this->moveAttribute = moveAttribute, this->attackAttribute = attackAttribute; }
	ServerAttribute *GetMoveAttribute(void) { return moveAttribute; }
	ServerAttribute *GetAttackAttribute(void) { return attackAttribute; }

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_PLAYER; }
};






/*******************************************************
  Client
********************************************************/
class ClientAttribute;
class AttackClientSide;
class ClientPlayer : public ClientMovableGameObject {
private:
	enum ENTRY_TYPE {
		NONE,
		ENTRY,
		SHOW,
		EXIT
	};

private:
	MultiAnimator anim;										// �A�j���[�V����
	MultiAnimator reverseAnim;								// ���]�A�j���[�V����
	MultiAnimator *curAnim = nullptr;						// �A�j���[�V�����i���݁j
	WIN::Time timer;										// �X�|�[���J�n������̎��Ԍv��
	ENTRY_TYPE entryType = NONE;							// ���ꉉ�o


	MultiAnimator allDamageEffect;							// �_���[�W�G�t�F�N�g
	MultiAnimator fireDamageEffect;							// ���_���[�W�G�t�F�N�g
	MultiAnimator waterDamageEffect;						// ���_���[�W�G�t�F�N�g
	MultiAnimator thunderDamageEffect;						// ���_���[�W�G�t�F�N�g
	MultiAnimator windDamageEffect;							// ���_���[�W�G�t�F�N�g
	MultiAnimator exEffect;									// ��������񂱃G�t�F�N�g
	MultiAnimator lvUpEffect;								// ���x���A�b�v�G�t�F�N�g
	MultiAnimator lvUpUI;									// ���x���A�b�vUI�G�t�F�N�g
	MultiAnimator lvDownUI;									// ���x���_�E��UI�G�t�F�N�g

public:
	int skillPoint = 0;										// �X�L���|�C���g
	int animType = ANIMATION_TYPE_IDLE;						// �A�j���[�V�����^�C�v
	int preAnimType = ANIMATION_TYPE_IDLE;					// �A�j���[�V�����i1�t���[���O�j
	int lv = 0;												// ���x��
	int preLv = 0;											// ���x���i1�t���[���O�j
	ATTRIBUTE_TYPE moveAttributeType;						// �ړ������^�C�v
	ATTRIBUTE_TYPE attackAttributeType;						// �U�������^�C�v
	ATTRIBUTE_TYPE preMoveAttributeType;					// �ړ������^�C�v
	ATTRIBUTE_TYPE preAttackAttributeType;					// �U�������^�C�v
	bool isReverseX = false;								// �����̌���
	Vector2 attackVelocity;									// �U���̃x���V�e�B
	Vector2 warpVelocity;									// ���[�v�x���V�e�B
	ClientAttribute *moveAttribute = nullptr;				// �ړ�����
	ClientAttribute *attackAttribute = nullptr;				// �U������
	ClientAttribute *curMoveAttribute = nullptr;			// �ړ������i���݁j
	ClientAttribute *curAttackAttribute = nullptr;			// �U�������i���݁j
	float skillPointAnimation = 0.0f;						// �X�L���|�C���g�l�����̃Q�[�W�̃A�j���[�V�����Ŏg�p����



public:
	ClientPlayer(ATTRIBUTE_TYPE moveAttributeType, ATTRIBUTE_TYPE attackAttributeType, Transform transform);

	void Loop(void) override;
	void ShowEntry();
	void ShowExit();

	void Update(ClientAttribute *moveAttribute, ClientAttribute *attackAttribute, MultiAnimator *anim);
	void SetMoveAttribute(ClientAttribute *moveAttribute);
	void SetAttackAttribute(ClientAttribute *attackAttribute);
	void SetAttribute(ClientAttribute *moveAttribute, ClientAttribute *attackAttribute) {
		SetMoveAttribute(moveAttribute);
		SetAttackAttribute(attackAttribute);
	}
	ClientAttribute *GetMoveAttribute(void) { return moveAttribute; }
	ClientAttribute *GetAttackAttribute(void) { return attackAttribute; }
};


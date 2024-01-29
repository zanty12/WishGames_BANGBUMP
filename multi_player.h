#pragma once
#include <sstream>
#include "multi_movable_object.h"
#include "multi_attribute.h"
#include "multi_anim.h"
#include "multi_map.h"
#include "attribute_type.h"
#include "multi_animenum.h"

/*******************************************************
  Server
********************************************************/
class ServerAttribute;
class AttackServerSide;
class ServerPlayer : public ServerMovableGameObject {
private:
	ServerAttribute *moveAttribute = nullptr;			// �ړ�����
	ServerAttribute *attackAttribute = nullptr;			// �U������


public:
	int skillPoint = 0;									// �X�L���|�C���g
	int score = 0;										// �X�R�A
	MultiMap *map = nullptr;							// �}�b�v
	MULTI_ANIMATION_TYPE animType = ANIMATION_TYPE_IDEL;// �A�j���[�V����
	Vector2 attackVelocity;								// �U���̃x���V�e�B
	Vector2 warpVelocity;								// ���[�v�x���V�e�B
	Vector2 blownVelocity;								// �_���[�W��������΂��ꂽ�x���V�e�B
	float blownFriction = 0.85f;						// �_���[�W��������΂��ꂽ�ۂ̌����x
	
public:
	ServerPlayer() { gravity = 0.5f; }
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
class ClientAttack;
class ClientPlayer : public ClientMovableGameObject {
private:
	ClientAttribute *moveAttribute = nullptr;			// �ړ�����
	ClientAttribute *attackAttribute = nullptr;			// �U������
	MultiAnimator reverseAnim;							// ���]�A�j���[�V����

public:
	int skillPoint = 0;										// �X�L���|�C���g
	MULTI_ANIMATION_TYPE animType = ANIMATION_TYPE_IDEL;	// �A�j���[�V�����^�C�v
	MULTI_ANIMATION_TYPE preAnimType = ANIMATION_TYPE_IDEL;	// �A�j���[�V�����i1�t���[���O�j
	ATTRIBUTE_TYPE moveAttributeType;					// �ړ������^�C�v
	ATTRIBUTE_TYPE attackAttributeType;					// �U�������^�C�v
	MultiAnimator anim;										// �A�j���[�V����
	bool isReverseX = false;								// �����̌���
	Vector2 attackVelocity;									// �U���̃x���V�e�B
	Vector2 warpVelocity;									// ���[�v�x���V�e�B


	
public:
	ClientPlayer(ATTRIBUTE_TYPE moveAttributeType, ATTRIBUTE_TYPE attackAttributeType, Transform transform);

	void Loop(void) override;

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

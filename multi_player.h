#pragma once
#include "multi_animenum.h"
#include "multi_object.h"
#include "multi_map.h"
#include "multi_attribute.h"

class MultiAttribute;
class ServerPlayer : public ServerGameObject {
private:
	MultiAttribute *moveAttribute = nullptr;			// �ړ�����
	MultiAttribute *attackAttribute = nullptr;			// �U������

public:
	int skillPoint = 0;									// �X�L���|�C���g
	MultiMap *map = nullptr;							// �}�b�v
	ANIMATION_TYPE animType = ANIMATION_TYPE_IDEL;		// �A�j���[�V����

	
	
public:
	~ServerPlayer() {
		if (moveAttribute) delete moveAttribute;
		if (attackAttribute) delete attackAttribute;
		moveAttribute = nullptr;
		attackAttribute = nullptr;
	}

	void SetMoveAttribute(MultiAttribute *moveAttribute) { this->moveAttribute = moveAttribute; }
	void SetAttackAttribute(MultiAttribute *attackAttribute) { this->attackAttribute = attackAttribute; }
	void SetAttackAttribute(MultiAttribute *moveAttribute, MultiAttribute *attackAttribute) { this->moveAttribute = moveAttribute, this->attackAttribute = attackAttribute; }
	MultiAttribute *GetMoveAttribute(void) { return moveAttribute; }
	MultiAttribute *GetAttackAttribute(void) { return attackAttribute; }

	void Update(void) override;
};

class ClientPlayer : public ClientGameObject {
public:
	int skillPoint = 0;									// �X�L���|�C���g
	ATTRIBUTE_TYPE moveAttribute;						// �ړ�����
	ATTRIBUTE_TYPE attackAttribute;						// �U������
	ANIMATION_TYPE animType = ANIMATION_TYPE_IDEL;		// �A�j���[�V����


	
public:
	ClientPlayer(Transform transform) : ClientGameObject(transform) { }
};
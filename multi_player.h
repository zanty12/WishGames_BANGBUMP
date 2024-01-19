#pragma once
#include <sstream>
#include "attribute_type.h"
#include "multi_animenum.h"
#include "multi_object.h"
#include "multi_map.h"
#include "multi_attribute.h"
#include "texture.h"

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

	void Loop(void) override;
};

class ClientPlayer : public ClientGameObject {
public:
	int skillPoint = 0;										// �X�L���|�C���g
	ATTRIBUTE_TYPE moveAttribute = ATTRIBUTE_TYPE_FIRE;		// �ړ�����
	ATTRIBUTE_TYPE attackAttribute = ATTRIBUTE_TYPE_FIRE;	// �U������
	ANIMATION_TYPE animType = ANIMATION_TYPE_IDEL;			// �A�j���[�V����


	
public:
	ClientPlayer(ATTRIBUTE_TYPE moveAttribute, ATTRIBUTE_TYPE attackAttribute, Transform transform)
		: moveAttribute(moveAttribute), attackAttribute(attackAttribute), ClientGameObject(transform) {
		// �p�X���w��
		std::ostringstream path;
		//path << "data/texture/Player/Player1_" << std::to_string((int)attackAttribute + 1) << std::to_string((int)moveAttribute + 1) << ".png";
		// �e�N�X�`����ǂݍ���
		//texNo = LoadTexture(path.str());

		texNo = LoadTexture("data/texture/player.png");
	}

	void Loop(void) override;
};
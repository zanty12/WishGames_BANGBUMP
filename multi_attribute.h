#pragma once
#include "lib/vector.h"
#include "multi_player.h"
#include "attribute_type.h"

class ServerPlayer;
class MultiAttribute {
protected:
	ServerPlayer *player = nullptr;

public:
	MultiAttribute(ServerPlayer* player) : player(player) { }
	virtual bool StickTrigger(Vector2 stick, Vector2 previousStick) = 0;
	virtual void Move(void) = 0;
	virtual void Attack(void) = 0;
	virtual ATTRIBUTE_TYPE GetAttribute(void) = 0;
};

class MultiAttack : public ServerGameObject {
private:
	ServerGameObject *self = nullptr;

public:
	MultiAttack(ServerGameObject *self) : self(self) { }

	const ServerGameObject *GetSelf(void) { return self; }
};



/*******************************************************
  Wind
********************************************************/
class MultiWind : public MultiAttribute {
private:
	float power_ = 0.0f;
	const float rotInputFriction = 0.5f; // �܂킷�����x�̖��C�萔
	const float rotInputJudgeMin = 0.1f; // �܂킷�𔻒肷��

	float prev_y_ = 0.0f;
	float previous_time_ = 0.0f;

	float maxPower_ = 10;
	float friction_ = 0.8f;
	MultiAttack *attack_ = nullptr;

public:
	MultiWind(ServerPlayer *player) : MultiAttribute(player) { }
	bool StickTrigger(Vector2 stick, Vector2 previousStick) override;
	void Move(void) override;
	void Attack(void) override;
	ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_WIND; };
};

class MultiWindAttack : public MultiAttack {
private:
	ServerGameObject *self = nullptr;

public:
	MultiWindAttack(ServerGameObject* self) : MultiAttack(self) { }

	const ServerGameObject *GetSelf(void) { return self; }
};




/*******************************************************
  Fire
********************************************************/
class MultiFire : public MultiAttribute {
private:
	float friction = 0.99f;				// ���C�W��
	float brakeFriction = 0.50f;		// ���C�W���i�u���[�L�j
	float maxSpeed = 5.0f;				// �����̍ő�l
	float judgeScale = 0.2f;			// �X�e�B�b�N�̌X�����Ƃ��ɔ��肷��ŏ��l
	Vector2 velocity;					// ����

	MultiAttack *attack_ = nullptr;

public:
	MultiFire(ServerPlayer *player) : MultiAttribute(player) { }
	bool StickTrigger(Vector2 stick, Vector2 previousStick) override;
	void Move(void) override;
	void Attack(void) override;
	ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_FIRE; };
};

class MultiFireAttack : public MultiAttack {
private:
	ServerGameObject *self = nullptr;

public:
	MultiFireAttack(ServerGameObject *self) : MultiAttack(self) { }

	const ServerGameObject *GetSelf(void) { return self; }
};
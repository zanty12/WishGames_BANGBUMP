#pragma once
#include <list>
#include "lib/vector.h"
#include "multi_anim.h"
#include "multi_player.h"
#include "multi_movable_object.h"
#include "multi_object.h"
#include "multi_attack.h"
#include "lib/collider2d.h"
#include "attribute_type.h"
#include "multi_path.h"
#include "ini.h"

/*******************************************************
  ����
********************************************************/
class ServerPlayer;
class ClientPlayer;
class ServerMovableGameObject;
class ClientMovableGameObject;
class ServerAttribute {
protected:
	ServerPlayer *player = nullptr;
	WIN::Time coolTimer;
	WIN::Time atkCoolTimer;
	WIN::Time atkAfterTimer;
	AttackServerSide *attack_ = nullptr;

public:
	float power = 0.0f;				// �p���[

	float minPower = 0.0f;			// �p���[�̍ŏ��l
	float maxPower = 10.0f;			// �p���[�̍ő�l
	float addPower = 00.0f;			// �p���[�̉��Z�l
	float friction = 0.98f;			// ���C�W��
	float powerFriction = 1.00f;	// �p���[�̖��C�W��
	float minInputDistance = 0.50f;	// ���͂̔���l
	float minInputSpeed = 0.50f;	// ���͂̉����̔���l
	float inputPowerRate = 1.0f;	// ���͒l���p���[�ɗ^����W��

	float atkRange = 1.0f;			// �U���͈�
	float atkDistance = 10.0f;		// �U������
	int atk = 0;					// �U����	
	int atkDrop = 0;				// �h���b�v��
	float coolTime = 0.0f;			// �N�[���^�C��
	float atkCoolTime = 1.0f;		// �U���N�[���^�C��
	float atkAfterTime = 0.0f;		// �U���̎c���
	float knockbackRate = 10.0f;	// �m�b�N�o�b�N��



public:
	ServerAttribute(ServerPlayer* player, std::wstring attributeName) : player(player) {
		minPower = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"minPower");
		maxPower = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"maxPower");
		addPower = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"addPower");
		friction = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"friction");
		powerFriction = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"powerFriction");
		minInputDistance = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"minInputDistance");
		minInputSpeed = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"minInputSpeed");
		inputPowerRate = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"inputPowerRate");


		atk = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"atk");
		atkDrop = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"atkDrop");
		atkCoolTime = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"atkCoolTime");
		atkAfterTime = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"atkAfterTime");
		coolTime = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"coolTime");
		atkRange = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"atkRange");
		atkDistance = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"atkDistance");
		knockbackRate = ini::GetFloat(PARAM_PATH + L"player.ini", attributeName, L"knockbackRate");

		coolTimer.Start();
	}
	virtual bool StickTrigger(Vector2 stick, Vector2 previousStick) = 0;
	virtual void Move(void) = 0;
	virtual void Attack(void) = 0;
	virtual AttackServerSide *CreateAttack(void);
	virtual void DestroyAttack(void);
	virtual ATTRIBUTE_TYPE GetAttribute(void) = 0;
	static ServerAttribute *Create(ServerPlayer *player, ATTRIBUTE_TYPE type);

	void AddPower(void);
	void AddPower(float scaler);
	void AddPower(Vector2 stick);
	void Friction(void);
	Vector2 CalcVector(Vector2 stick);
};
class ClientAttribute {
protected:
	ClientPlayer *player = nullptr;
	MultiAnimator attackAnim;
	MultiAnimator moveAnim;

public:
	ClientAttribute(ClientPlayer *player) : player(player) { }
	virtual void Move(void) = 0;
	virtual void Attack(void) = 0;
	virtual void Idle(void) { }
	virtual ATTRIBUTE_TYPE GetAttribute(void) = 0;
	static ClientAttribute *Create(ClientPlayer *player, ATTRIBUTE_TYPE type);
};



/*******************************************************
  Fire
********************************************************/
class ServerFire : public ServerAttribute {
private:
	float brakeFriction = 0.50f;		// ���C�W���i�u���[�L�j
	Vector2 velocity;					// ����

public:
	ServerFire(ServerPlayer *player) : ServerAttribute(player, L"Fire") { }
	bool StickTrigger(Vector2 stick, Vector2 previousStick = Vector2::Zero) override;
	void Move(void) override;
	void Attack(void) override;
	AttackServerSide *CreateAttack(void) override;
	ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_FIRE; };
};
class ClientFire : public ClientAttribute {
private:
	static const int ANIM_NUM = 50;
	static const int FRAME_NUM = 24;
	struct Animator {
		Vector2 pos;
		float rot = 0.0f;
		Vector2 scl;
		MultiAnimator anim;
	};
	int moveTexNo = -1;
	int attackTexNo = -1;
	std::list<Animator> moveAnims;
	std::list<Animator> attackAnims;
	DWORD startTime = 0;

public:


	ClientFire(ClientPlayer *player) : ClientAttribute(player) {
		moveTexNo = LoadTexture("data/texture/Effect/effect_fire_move.png");
		attackTexNo = LoadTexture("data/texture/Effect/effect_fire_attack.png");

		moveAnim = MultiAnimator(moveTexNo, 5, 6, 0, 25, true);
		attackAnim = MultiAnimator(attackTexNo, 5, 6, 0, 29, true, 0, 25);
		startTime = timeGetTime();
	}

	void Move(void) override;
	void Attack(void) override;
	virtual ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_FIRE; };
};

class ServerFireAttack : public AttackServerSide {
public:
	ServerFireAttack(GameObjectServerSide *self, ServerAttribute *attribute) :		
		AttackServerSide(attribute->atk, attribute->atkDrop, attribute->atkCoolTime, attribute->knockbackRate, attribute->atkRange, self) { }

	void Loop(void) override;
	void KnockBack(ServerMovableGameObject *object) override;

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_FIRE; }
};





/*******************************************************
  Water
********************************************************/
class ServerWater : public ServerAttribute {
public:
	ServerWater(ServerPlayer *player) : ServerAttribute(player, L"Water") { }
	bool StickTrigger(Vector2 stick, Vector2 previousStick) override;
	void Move(void) override;
	void Attack(void) override;
	AttackServerSide *CreateAttack(void) override;
	ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_DARK; };
};
class ClientWater : public ClientAttribute {
public:
	MultiAnimator moveChargeAnim;
	MultiAnimator idle;
	MultiAnimator indicator;
	Vector2 prevPosition;			// ���[�v�O�̍��W


	ClientWater(ClientPlayer *player) : ClientAttribute(player) {
		moveAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_water_move.png"), 5, 3, 0, 14, false);
		attackAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_water_attack.png"), 5, 6, 0, 29, true);
		moveChargeAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_water_charge.png"), 5, 10, 0, 47, true);
		idle = MultiAnimator(LoadTexture("data/texture/Effect/effect_water_idle.png"), 5, 6, 0, 29, true);
		indicator = MultiAnimator(LoadTexture("data/texture/Effect/UI_water_indicator.png"), 5, 4, 0, 18, true);

		moveAnim.MoveEnd();
	}

	void Move(void) override;
	void Attack(void) override;
	void Idle(void) override;
	virtual ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_DARK; };
};

class ServerWaterAttack : public AttackServerSide {
public:
	ServerWaterAttack(GameObjectServerSide *self, ServerAttribute *attribute) :
		AttackServerSide(attribute->atk, attribute->atkDrop, attribute->atkCoolTime, attribute->knockbackRate, attribute->atkRange, self) { }

	void Loop(void) override;
	void KnockBack(ServerMovableGameObject *object) override;

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_WATER; }
};





/*******************************************************
  Thunder
********************************************************/
class ServerThunder : public ServerAttribute {
private:
	float brakeFriction = 0.50f;		// ���C�W���i�u���[�L�j

public:
	ServerThunder(ServerPlayer *player) : ServerAttribute(player, L"Thunder") { }
	bool StickTrigger(Vector2 stick, Vector2 previousStick) override;
	void Move(void) override;
	void Attack(void) override;
	AttackServerSide *CreateAttack(void) override;
	ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_THUNDER; };
};
class ClientThunder : public ClientAttribute {
private:
	int moveTexNo = -1;
	int attackTexNo = -1;

public:


	ClientThunder(ClientPlayer *player) : ClientAttribute(player) {
		moveTexNo = LoadTexture("data/texture/Effect/effect_fire_move.png");
		attackTexNo = LoadTexture("data/texture/Effect/effect_fire_attack.png");

		moveAnim = MultiAnimator(moveTexNo, 5, 6, 0, 25, true);
		attackAnim = MultiAnimator(attackTexNo, 5, 6, 0, 29, true);
	}

	void Move(void) override;
	void Attack(void) override;
	virtual ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_THUNDER; };
};

class ServerThunderAttack : public AttackServerSide {
public:
	float gravity = 0.1f;

	ServerThunderAttack(GameObjectServerSide *self, ServerAttribute *attribute) :
		AttackServerSide(attribute->atk, attribute->atkDrop, attribute->atkCoolTime, attribute->knockbackRate, attribute->atkRange, self) {
		transform.position = self->transform.position;
	}

	void Loop(void) override;
	void KnockBack(ServerMovableGameObject *object) override;

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_THUNDER; }
};
class ClientThunderAttack : public AttackClientSide {
public:
	float gravity = 0.1f;
	MultiAnimator anim;

	ClientThunderAttack(Transform transform) : AttackClientSide(transform) {
		texNo = LoadTexture("data/texture/Effect/effect_thunder_arrow.png");
		anim = MultiAnimator(texNo, 5, 6, 0, 29, true);
	}

	void Loop(void) override;

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_THUNDER; }
};





/*******************************************************
  Wind
********************************************************/
class ServerWind : public ServerAttribute {
private:
	float prev_y_ = 0.0f;
	float previous_time_ = 0.0f;

public:
	ServerWind(ServerPlayer *player) : ServerAttribute(player, L"Wind") { }
	bool StickTrigger(Vector2 stick, Vector2 previousStick) override;
	void Move(void) override;
	void Attack(void) override;
	AttackServerSide *CreateAttack(void) override;
	ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_WIND; };
};
class ClientWind : public ClientAttribute {
public:
	MultiAnimator idle;


	ClientWind(ClientPlayer *player) : ClientAttribute(player) {
		attackAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_wind_attack.png"), 5, 6, 0, 29, true);
		moveAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_wind_attack.png"), 5, 6, 0, 25, true);
		idle = MultiAnimator(LoadTexture("data/texture/Effect/effect_wind_idle.png"), 5, 6, 0, 29, true);
	}

	void Move(void) override;
	void Attack(void) override;
	void Idle(void) override;
	virtual ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_WIND; };
};

class ServerWindAttack : public AttackServerSide {
public:
	ServerWindAttack(GameObjectServerSide *self, ServerAttribute *attribute) :
		AttackServerSide(attribute->atk, attribute->atkDrop, attribute->atkCoolTime, attribute->knockbackRate, attribute->atkRange, self) { }

	void Loop(void) override; 
	void KnockBack(ServerMovableGameObject *object) override;

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_WIND; }
};


#pragma once
#include <list>
#include "lib/vector.h"
#include "multi_anim.h"
#include "multi_player.h"
#include "multi_object.h"
#include "multi_attack.h"
#include "attribute_type.h"

/*******************************************************
  属性
********************************************************/
class ServerPlayer;
class ClientPlayer;
class ServerAttribute {
protected:
	ServerPlayer *player = nullptr;

public:
	ServerAttribute(ServerPlayer* player) : player(player) { }
	virtual bool StickTrigger(Vector2 stick, Vector2 previousStick) = 0;
	virtual void Move(void) = 0;
	virtual void Attack(void) = 0;
	virtual ATTRIBUTE_TYPE GetAttribute(void) = 0;
	static ServerAttribute *Create(ServerPlayer *player, ATTRIBUTE_TYPE type);
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
	virtual ATTRIBUTE_TYPE GetAttribute(void) = 0;
	static ClientAttribute *Create(ClientPlayer *player, ATTRIBUTE_TYPE type);
};



/*******************************************************
  Wind
********************************************************/
class ServerWind : public ServerAttribute {
private:
	float power_ = 0.0f;
	const float rotInputFriction = 0.5f; // まわす加速度の摩擦定数
	const float rotInputJudgeMin = 0.1f; // まわすを判定する

	float prev_y_ = 0.0f;
	float previous_time_ = 0.0f;

	float maxPower_ = 10;
	float friction_ = 0.8f;
	ServerAttack *attack_ = nullptr;

public:
	ServerWind(ServerPlayer *player) : ServerAttribute(player) { }
	bool StickTrigger(Vector2 stick, Vector2 previousStick) override;
	void Move(void) override;
	void Attack(void) override;
	ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_WIND; };
};
class ClientWind : public ClientAttribute {
public:
	ClientWind(ClientPlayer *player) : ClientAttribute(player) {
		attackAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_wind_attack.png"), 5, 6, 0, 29);
		moveAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_wind_attack.png"), 5, 6, 0, 25);
	}

	void Move(void) override;
	void Attack(void) override;
	virtual ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_WIND; };
};

class ServerWindAttack : public ServerAttack {
private:
	ServerGameObject *self = nullptr;

public:
	ServerWindAttack(ServerGameObject* self) : ServerAttack(1, 1, 100, self) { }

	const ServerGameObject *GetSelf(void) { return self; }

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_WIND; }
};
class ClientWindAttack : public ClientAttack {
public:
	void Loop(void) override;

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_WIND; }
};




/*******************************************************
  Fire
********************************************************/
class ServerFire : public ServerAttribute {
private:
	float friction = 0.99f;				// 摩擦係数
	float brakeFriction = 0.50f;		// 摩擦係数（ブレーキ）
	float maxSpeed = 5.0f;				// 加速の最大値
	float judgeScale = 0.2f;			// スティックの傾けたときに判定する最小値
	Vector2 velocity;					// 向き

	ServerAttack *attack_ = nullptr;

public:
	ServerFire(ServerPlayer *player) : ServerAttribute(player) { }
	bool StickTrigger(Vector2 stick, Vector2 previousStick) override;
	void Move(void) override;
	void Attack(void) override;
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

		moveAnim = MultiAnimator(moveTexNo, 5, 6, 0, 25, true, 0, 25);
		attackAnim = MultiAnimator(attackTexNo, 5, 6, 0, 29, true, 0, 25);
		startTime = timeGetTime();
	}

	void Move(void) override;
	void Attack(void) override;
	virtual ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_FIRE; };
};

class ServerFireAttack : public ServerAttack {
private:
	ServerGameObject *self = nullptr;

public:
	ServerFireAttack(ServerGameObject *self) : ServerAttack(1, 1, 50, self) { }

	const ServerGameObject *GetSelf(void) { return self; }

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_FIRE; }
};

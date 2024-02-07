#pragma once
#include <list>
#include "lib/vector.h"
#include "lib/get_set.h"
#include "asset.h"
#include "multi_anim.h"
#include "multi_player.h"
#include "multi_movable_object.h"
#include "multi_object.h"
#include "multi_attack.h"
#include "lib/collider2d.h"
#include "attribute_type.h"
#include "attribute_state.h"
#define MAX_LV 10

/*******************************************************
  属性
********************************************************/
class ServerPlayer;
class ClientPlayer;
class ServerMovableGameObject;
class ClientMovableGameObject;
class ServerAttribute {
public:
	ServerPlayer *player = nullptr;
	WIN::Time coolTimer;
	WIN::Time atkCoolTimer;
	WIN::Time atkAfterTimer;
	WIN::Time skillMpTimer;
	AttackServerSide *attack_ = nullptr;

public:
	float power = 0.0f;									// パワー
	int mp = 0;											// マナ
	int lv = 0;											// レベル
	AttributeState *state = nullptr;					// 現在のステータス
	AttributeState state_lv[MAX_LV] = {};				// ステータス


public:
	ServerAttribute(ServerPlayer* player, std::wstring attributeName) : player(player) {
		// ステータスを読み込む
		std::wstring lvStr = L"lv";
		for (int i = 0; i < MAX_LV; i++) {
			state_lv[i] = AttributeState(attributeName, i + 1);
		}
		// レベル1のステータスにする
		state = state_lv;

		coolTimer.Start();
		skillMpTimer.Start();
	}
	virtual bool StickTrigger(Vector2 stick, Vector2 previousStick) = 0;
	virtual void Move(void) = 0;
	virtual void Attack(void) = 0;
	virtual AttackServerSide *CreateAttack(void);
	virtual bool DestroyAttack(void);
	virtual ATTRIBUTE_TYPE GetAttribute(void) = 0;
	void LevelUpdate(void);
	void MpUpdate(void);
	bool IsUseMp(void);
	static ServerAttribute *Create(ServerPlayer *player, ATTRIBUTE_TYPE type);

	void AddPower(void);
	void AddPower(float scaler);
	void AddPower(Vector2 stick);
	void FrictionPower(void);
	void Friction(void);
	Vector2 CalcVector(Vector2 stick);
};
class ClientAttribute {
protected:
	ClientPlayer *player = nullptr;
	MultiAnimator attackAnim;
	MultiAnimator moveAnim;
	int attackTexNo = -1;
	int attack2TexNo = -1;
	int moveTexNo = -1;
	int frameUITexNo = -1;
	int uiTexNo = -1;

	AttributeState *state = nullptr;		// 現在のステータス
	AttributeState state_lv[MAX_LV] = {};	// ステータス

public:
	float power = 0.0f;									// パワー
	int mp = 0;											// マナ



public:
	ClientAttribute(ClientPlayer *player, std::wstring attributeName) : player(player) {
		// ステータスを読み込む
		std::wstring lvStr = L"lv";
		for (int i = 0; i < MAX_LV; i++) {
			state_lv[i] = AttributeState(attributeName, i + 1);
		}
		// レベル1のステータスにする
		state = state_lv;
	}
	virtual void Move(void) = 0;
	virtual void Attack(void) = 0;
	virtual void Idle(void) { }
	virtual void DrawUI(void) { };
	virtual ATTRIBUTE_TYPE GetAttribute(void) = 0;
	static ClientAttribute *Create(ClientPlayer *player, ATTRIBUTE_TYPE type);
	AttributeState *GetState(void) { return state; }
	AttributeState *GetState(int idx) { return &state_lv[idx]; }
	void LevelUpdate(void);
};



/*******************************************************
  Fire
********************************************************/
class ServerFire : public ServerAttribute {
private:
	float brakeFriction = 0.50f;		// 摩擦係数（ブレーキ）
	Vector2 velocity;					// 向き

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
	std::list<Animator> moveAnims;
	DWORD startTime = 0;

public:


	ClientFire(ClientPlayer *player) : ClientAttribute(player, L"Fire") {
		moveTexNo = LoadTexture(Asset::GetAsset(textures::fire_move));
		attackTexNo = LoadTexture(Asset::GetAsset(textures::fire_attack));
		attack2TexNo = LoadTexture("data/texture/Attack/effect_fire_attack2.png");
		frameUITexNo = LoadTexture("data/texture/UI/UI_fire_cooldown.png");
		uiTexNo = LoadTexture("data/texture/UI/UI_fire_cooldown2.png");

		moveAnim = MultiAnimator(moveTexNo, 5, 6, 0, 25, true);
		attackAnim = MultiAnimator(attackTexNo, 5, 6, 0, 25, true);
		startTime = timeGetTime();
	}

	void Move(void) override;
	void Attack(void) override;
	void DrawUI(void) override;
	virtual ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_FIRE; };
};

class ServerFireAttack : public AttackServerSide {
public:
	ServerFireAttack(GameObjectServerSide *self, ServerAttribute *attribute) :		
		AttackServerSide(attribute->state->atk, attribute->state->atkDrop, attribute->state->atkCoolTime, attribute->state->knockbackRate, attribute->state->atkRange, self) { }

	void Loop(void) override;
	void KnockBack(ServerMovableGameObject *object) override;

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_FIRE; }
};





/*******************************************************
  Water
********************************************************/
class ServerWater : public ServerAttribute {
	WIN::Time attackTimer;

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
	MultiAnimator attackChargeAnim;
	MultiAnimator idle;
	MultiAnimator indicator;
	Vector2 prevPosition;			// ワープ前の座標


	ClientWater(ClientPlayer *player) : ClientAttribute(player, L"Water") {
		moveTexNo = LoadTexture(Asset::GetAsset(textures::dark_move));
		attackTexNo = LoadTexture(Asset::GetAsset(textures::dark_attack));
		attack2TexNo = LoadTexture("data/texture/Attack/effect_water_attack2.png");
		frameUITexNo = LoadTexture("data/texture/UI/UI_water_cooldown.png");
		uiTexNo = LoadTexture("data/texture/UI/UI_water_cooldown2.png");
		int moveChargeTexNo = LoadTexture(Asset::GetAsset(textures::dark_move_charge));
		int attackChargeTexNo = LoadTexture("data/texture/Effect/effect_water_attack_charge.png");
		int idleTexNo = LoadTexture(Asset::GetAsset(textures::dark_idle));
		int indicatorTexNo = LoadTexture("data/texture/Effect/UI_water_indicator.png");

		moveAnim = MultiAnimator(moveTexNo, 5, 3, 0, 14, false);
		attackAnim = MultiAnimator(attackTexNo, 5, 6, 0, 29, true);
		moveChargeAnim = MultiAnimator(moveChargeTexNo, 5, 10, 0, 47, true);
		attackChargeAnim = MultiAnimator(attackChargeTexNo, 5, 10, 0, 47, true);
		idle = MultiAnimator(idleTexNo, 5, 6, 0, 29, true);
		indicator = MultiAnimator(indicatorTexNo, 5, 4, 0, 18, true);

		moveAnim.MoveEnd();
	}

	void Move(void) override;
	void Attack(void) override;
	void Idle(void) override;
	void DrawUI(void) override;
	virtual ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_DARK; };
};

class ServerWaterAttack : public AttackServerSide {
private:
	ServerAttribute *attribute = nullptr;

public:
	ServerWaterAttack(GameObjectServerSide *self, ServerAttribute *attribute) : attribute(attribute),
		AttackServerSide(attribute->state->atk, attribute->state->atkDrop, attribute->state->atkCoolTime, attribute->state->knockbackRate, attribute->state->atkRange, self) {
	}

	void Loop(void) override;
	void KnockBack(ServerMovableGameObject *object) override;

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_WATER; }
};





/*******************************************************
  Thunder
********************************************************/
class ServerThunder : public ServerAttribute {
private:
	float brakeFriction = 0.50f;		// 摩擦係数（ブレーキ）
	WIN::Time chargeToAttackTimer;		// チャージアニメーションから攻撃アニメーションに切り替わったときの時間
	float friction = 0.0f;

public:
	ServerThunder(ServerPlayer *player) : ServerAttribute(player, L"Thunder") { chargeToAttackTimer.Start(); }
	bool StickTrigger(Vector2 stick, Vector2 previousStick) override;
	void Move(void) override;
	void Attack(void) override;
	AttackServerSide *CreateAttack(void) override;
	ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_THUNDER; };
};
class ClientThunder : public ClientAttribute {
public:
	MultiAnimator chargeAttackAnim;


	ClientThunder(ClientPlayer *player) : ClientAttribute(player, L"Thunder") {
		moveAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_thunder_move.png"), 5, 3, 0, 14, false);
		chargeAttackAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_thunder_charge.png"), 5, 3, 0, 14, true);
		frameUITexNo = LoadTexture("data/texture/UI/UI_thunder_cooldown.png");
		uiTexNo = LoadTexture("data/texture/UI/UI_thunder_cooldown2.png");
	}

	void Move(void) override;
	void Attack(void) override;
	void DrawUI(void) override;
	virtual ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_THUNDER; };
};

class ServerThunderAttack : public AttackServerSide {
public:
	float gravity = 0.0f;

	ServerThunderAttack(GameObjectServerSide *self, ServerAttribute *attribute) :
		AttackServerSide(attribute->state->atk, attribute->state->atkDrop, attribute->state->atkCoolTime, attribute->state->knockbackRate, attribute->state->atkRange, self) {
		transform.position = self->transform.position;
	}

	void Loop(void) override;
	void KnockBack(ServerMovableGameObject *object) override;

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_THUNDER; }
};
class ServerThunder2Attack : public ServerThunderAttack {
public:
	float gravity = 0.0f;

	ServerThunder2Attack(GameObjectServerSide *self, ServerAttribute *attribute) :
		ServerThunderAttack(self, attribute) {
		transform.position = self->transform.position;
	}

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_THUNDER; }
};
class ClientThunderAttack : public AttackClientSide {
public:
	float gravity = 0.0f;
	MultiAnimator anim;
	MultiAnimator deathAnim;

	ClientThunderAttack(Transform transform) : AttackClientSide(transform) {
		texNo = LoadTexture("data/texture/Attack/effect_thunder_arrow.png");
		anim = MultiAnimator(texNo, 5, 6, 0, 29, true);
		deathAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_thunder_lost.png"), 5, 2, 0, 4, false);
		this->transform.scale = Vector2::One * 150.0f;
	}

	void Loop(void) override;
	void Release(void) override;

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_THUNDER; }
};
class ClientThunder2Attack : public ClientThunderAttack {
public:
	float gravity = 0.0f;
	MultiAnimator anim;

	ClientThunder2Attack(Transform transform) : ClientThunderAttack(transform) {
		texNo = LoadTexture("data/texture/Attack/effect_thunder_arrow2.png");
		anim = MultiAnimator(texNo, 5, 6, 0, 29, true);
	}

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_THUNDER2; }
};





/*******************************************************
  Wind
********************************************************/
class ServerWind : public ServerAttribute {
private:
	float horizontalVelocity = 0.0f;
	float maxHorizontalVelocity = 10.0f;

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
	int prevAnimType = ANIMATION_TYPE_IDLE;
	Vector2 prevPosition;			// ワープ前の座標

	ClientWind(ClientPlayer *player) : ClientAttribute(player, L"Wind") {
		moveTexNo = LoadTexture(Asset::GetAsset(textures::wind_move));
		attackTexNo = LoadTexture(Asset::GetAsset(textures::wind_attack));
		attack2TexNo = LoadTexture("data/texture/Attack/effect_wind_attack2.png");
		frameUITexNo = LoadTexture("data/texture/UI/UI_wind_cooldown.png");
		uiTexNo = LoadTexture("data/texture/UI/UI_wind_cooldown2.png");

		attackAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_wind_attack.png"), 5, 6, 0, 29, true);
		moveAnim = MultiAnimator(LoadTexture("data/texture/Effect/effect_wind_move.png"), 5, 6, 0, 29, false);
		idle = MultiAnimator(LoadTexture("data/texture/Effect/effect_wind_idle.png"), 5, 6, 0, 29, true);
	}

	void Move(void) override;
	void Attack(void) override;
	void Idle(void) override;
	void DrawUI(void) override;
	virtual ATTRIBUTE_TYPE GetAttribute(void) override { return ATTRIBUTE_TYPE_WIND; };
};

class ServerWindAttack : public AttackServerSide {
public:
	ServerWindAttack(GameObjectServerSide *self, ServerAttribute *attribute) :
		AttackServerSide(attribute->state->atk, attribute->state->atkDrop, attribute->state->atkCoolTime, attribute->state->knockbackRate, attribute->state->atkRange, self) { }

	void Loop(void) override; 
	void KnockBack(ServerMovableGameObject *object) override;

	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_ATTACK_WIND; }
};

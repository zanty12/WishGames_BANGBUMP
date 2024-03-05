#pragma once
#include "multi_movable_object.h"
#include "multiplay.h"
#include "multi_map.h"
#include "multi_attack.h"
#include "multi_anim.h"

class MultiMap;
class EnemyAttackServerSide : public AttackServerSide {
public:

	EnemyAttackServerSide() = default;
	EnemyAttackServerSide(int atk, int atkDrop, float spanTime, float knockbackRate, float radius, GameObjectServerSide *self) : AttackServerSide(atk, atkDrop, spanTime, knockbackRate, radius, self) { }

	MULTI_OBJECT_TYPE GetType(void) override {
		return MULTI_ATTACK_ENEMY2;
	};

};

class EnemyServerSide : public ServerMovableGameObject {
protected:
	MultiMap *map = nullptr;
	float speed = 2.0f;
	EnemyAttackServerSide attack;

	int hp = 5;
	int atkDrop = 5;
	int deathDrop = 5;
	int score = 1;
	float knockbackRate = 5.0f;



public:
	EnemyServerSide(Transform transform, MultiMap* map, std::wstring enemyName) : map(map), ServerMovableGameObject(transform) {
		hp = ini::GetFloat(PARAM_PATH + L"enemy.ini", enemyName, L"hp");
		score = ini::GetFloat(PARAM_PATH + L"enemy.ini", enemyName, L"score");
		radius = ini::GetFloat(PARAM_PATH + L"enemy.ini", enemyName, L"radius") * 0.25f;
		atkDrop = ini::GetFloat(PARAM_PATH + L"enemy.ini", enemyName, L"atkDrop");
		deathDrop = ini::GetFloat(PARAM_PATH + L"enemy.ini", enemyName, L"drop");
		knockbackRate = ini::GetFloat(PARAM_PATH + L"enemy.ini", enemyName, L"knockbackRate");
		attack = EnemyAttackServerSide(0, atkDrop, 0, knockbackRate, radius, this);
	}
	void Damage(AttackServerSide *attack) override;
	void BlownPlayers(void);
	AttackServerSide *GetAttack(void) { return &attack; }
};
class EnemyClientSide : public ClientMovableGameObject {
protected:
	MultiAnimator anim;
	MultiAnimator deathAnim;

	MultiAnimator allDamageEffect;							// ダメージエフェクト
	MultiAnimator fireDamageEffect;							// 炎ダメージエフェクト
	MultiAnimator waterDamageEffect;						// 水ダメージエフェクト
	MultiAnimator thunderDamageEffect;						// 雷ダメージエフェクト
	MultiAnimator windDamageEffect;							// 風ダメージエフェクト

	int deathSeNo = -1;
public:

	EnemyClientSide(Transform transform, std::wstring enemyName) : ClientMovableGameObject(transform) {
		radius = ini::GetFloat(PARAM_PATH + L"enemy.ini", enemyName, L"radius");
		transform.scale = Vector2::One * radius;
		deathAnim = MultiAnimator(LoadTexture(Asset::GetAsset(textures::effect_enemydead)), 5, 6, 0, 29, false);

		deathSeNo = LoadSound("data/sound/SE/enemy_death.wav");

		// ダメージエフェクト
		allDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_all.png"), 5, 2, 0, 7, false);
		fireDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_fire.png"), 5, 2, 0, 7, false);
		waterDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_water.png"), 5, 2, 0, 7, false);
		thunderDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_thunder.png"), 5, 2, 0, 7, false);
		windDamageEffect = MultiAnimator(LoadTexture("data/texture/Effect/effect_hit_wind.png"), 5, 2, 0, 7, false);
	}

	void DamageEffectUpdate(void);
	void Release(void) override;
};




class Enemy1ServerSide : public EnemyServerSide {
public:
	Enemy1ServerSide(Transform transform, MultiMap *map) : EnemyServerSide(transform, map, L"Enemy1") {
		velocity = Vector2::Left * speed;
	}

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY1; }
};
class Enemy1ClientSide : public EnemyClientSide {
public:
	Enemy1ClientSide(Transform transform) : EnemyClientSide(transform, L"Enemy1") {
		texNo = LoadTexture("data/texture/Enemy/enemy1_anim.png");
		anim = MultiAnimator(texNo, 5, 4, 0, 17, true);
		anim.SetFrame(1000 / 42);
	}

	void Loop(void) override;
};



class Enemy2ServerSide : public EnemyServerSide {
	float activeRadius = 1000.0f;		// 検知範囲
	float coolTime = 5.0f;
	WIN::Time spawnTimer;

public:
	Enemy2ServerSide(Transform transform, MultiMap *map) : EnemyServerSide(transform, map, L"Enemy2") {
		spawnTimer.Start();
	}

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY2; }
};
class Enemy2ClientSide : public EnemyClientSide {
public:
	Enemy2ClientSide(Transform transform) : EnemyClientSide(transform, L"Enemy2") {
		texNo = LoadTexture("data/texture/Enemy/enemy2_anim.png");
		anim = MultiAnimator(texNo, 5, 6, 0, 29, true);
		anim.SetFrame(1000 / 42);
	}

	void Loop(void) override;
};
class AttackEnemy2ServerSide : public AttackServerSide {
public:
	AttackEnemy2ServerSide(Enemy2ServerSide *self) : AttackServerSide(1, self->GetAttack()->atkDrop, 0.0f, self->blownFriction, 50.0f, self) { transform.position = self->transform.position; isProjectile = true; }

	void Loop(void) override;
	void KnockBack(ServerMovableGameObject *object) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ATTACK_ENEMY2; }
};
class AttackEnemy2ClientSide : public AttackClientSide {
	MultiAnimator anim;
public:
	AttackEnemy2ClientSide(Transform transform) : AttackClientSide(transform) {
		texNo = LoadTexture("data/texture/Effect/effect_enemy2_attack.png");
		anim = MultiAnimator(texNo, 5, 6, 0, 29, true);
		radius = 100.0f;
	}

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ATTACK_ENEMY2; }
};




class Enemy3ServerSide : public EnemyServerSide {
public:
	float activeRadius = 1000.0f;		// 検知範囲
	Enemy3ServerSide(Transform transform, MultiMap *map) : EnemyServerSide(transform, map, L"Enemy3") {
	}

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY3; }
};
class Enemy3ClientSide : public EnemyClientSide {
protected:
	float speed = 10.0f;

public:



public:
	Enemy3ClientSide(Transform transform) : EnemyClientSide(transform, L"Enemy3") {
		texNo = LoadTexture("data/texture/Enemy/enemy3_anim.png");
		anim = MultiAnimator(texNo, 5, 6, 0, 29, true);
		anim.SetFrame(1000 / 42);
	}

	void Loop(void) override;
};

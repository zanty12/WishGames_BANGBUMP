#pragma once
#include "multi_movable_object.h"
#include "multiplay.h"
#include "multi_map.h"
#include "multi_attack.h"
#include "multi_anim.h"

class MultiMap;
class EnemyServerSide : public ServerMovableGameObject {
protected:
	MultiMap *map = nullptr;
	float speed = 2.0f;

public:
	int hp = 5;
	int atkDrop = 5;



public:
	EnemyServerSide(Transform transform, MultiMap* map) : map(map), ServerMovableGameObject(transform) { }
	void Damage(AttackServerSide *attack) override;
	void BlownPlayers(void);
};
class EnemyClientSide : public ClientMovableGameObject {
protected:
	MultiAnimator anim;

public:
	EnemyClientSide(Transform transform) : ClientMovableGameObject(transform) { }
};




class Enemy1ServerSide : public EnemyServerSide {
public:
	Enemy1ServerSide(Transform transform, MultiMap *map) : EnemyServerSide(transform, map) {
		velocity = Vector2::Left * speed;
		radius = 100.0f;
	}

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY1; }
};
class Enemy1ClientSide : public EnemyClientSide {
public:
	Enemy1ClientSide(Transform transform) : EnemyClientSide(transform) {
		texNo = LoadTexture("data/texture/Enemy/enemy1_anim.png");
		anim = MultiAnimator(texNo, 5, 4, 0, 17, true);
	}

	void Loop(void) override;
};



class Enemy2ServerSide : public EnemyServerSide {
	float coolTime = 1.0f;
	WIN::Time spawnTimer;

public:
	Enemy2ServerSide(Transform transform, MultiMap *map) : EnemyServerSide(transform, map) {
		radius = 100.0f; 
		spawnTimer.Start();
	}

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY2; }
};
class Enemy2ClientSide : public EnemyClientSide {
public:
	Enemy2ClientSide(Transform transform) : EnemyClientSide(transform) {
		texNo = LoadTexture("data/texture/Enemy/enemy2_anim.png");
		anim = MultiAnimator(texNo, 5, 6, 0, 29, true);
	}

	void Loop(void) override;
};
class AttackEnemy2ServerSide : public AttackServerSide {
public:
	AttackEnemy2ServerSide(Enemy2ServerSide *self) : AttackServerSide(1, 5, 1.0f, 5, 100.0f, self) { transform.position = self->transform.position; }

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
	}

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ATTACK_ENEMY2; }
};




class Enemy3ServerSide : public EnemyServerSide {
public:
	float activeRadius = 1000.0f;		// ŒŸ’m”ÍˆÍ
	Enemy3ServerSide(Transform transform, MultiMap *map) : EnemyServerSide(transform, map) {
		radius = 100.0f;
	}

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY3; }
};
class Enemy3ClientSide : public EnemyClientSide {
protected:
	float speed = 10.0f;

public:



public:
	Enemy3ClientSide(Transform transform) : EnemyClientSide(transform) {
		texNo = LoadTexture("data/texture/Enemy/enemy3_anim.png");
		anim = MultiAnimator(texNo, 5, 6, 0, 29, true);
	}

	void Loop(void) override;
};

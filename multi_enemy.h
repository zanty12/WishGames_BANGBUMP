#pragma once
#include "multi_object.h"
#include "multiplay.h"
#include "multi_map.h"
#include "multi_anim.h"

class MultiMap;
class EnemyServerSide : public GameObjectServerSide {
protected:
	MultiMap *map = nullptr;
	float speed = 10.0f;

public:
	int hp = 5;
	int atkDrop = 5;



public:
	EnemyServerSide(Transform transform, MultiMap* map) : map(map), GameObjectServerSide(transform) { }
	void BlownPlayers(void);
};
class EnemyClientSide : public GameObjectClientSide {
protected:
	MultiAnimator anim;



public:
};




class Enemy1ServerSide : public EnemyServerSide {
public:
	Enemy1ServerSide(Transform transform, MultiMap *map) : EnemyServerSide(transform, map) {
		velocity = Vector2::Left * speed;
		radius = 50.0f;
	}

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY1; }
};

class Enemy2ServerSide : public EnemyServerSide {
public:
	Enemy2ServerSide(Transform transform, MultiMap *map) : EnemyServerSide(transform, map) { }

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY2; }
};

class Enemy3ServerSide : public EnemyServerSide {
public:
	float activeRadius = 50.0f;		// ŒŸ’m”ÍˆÍ
	Enemy3ServerSide(Transform transform, MultiMap *map) : EnemyServerSide(transform, map) { }

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY3; }
};
class Enemy1ClientSide : public EnemyClientSide {
protected:
	float speed = 10.0f;

public:



public:
	Enemy1ClientSide() {
		texNo = LoadTexture("data/texture/Enemy/enemy1_anim.png");
		anim = MultiAnimator(texNo, 5, 4, 0, 17, true);
	}

	void Loop(void) override;
};
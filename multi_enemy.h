#pragma once
#include "multi_object.h"
#include "multi_map.h"

class MultiMap;
class EnemyServerSide : public GameObjectServerSide {
protected:
	MultiMap *map = nullptr;
	float speed = 10.0f;

public:
	int hp = 5;



public:
	EnemyServerSide(MultiMap* map) : map(map) { }
};
class EnemyClientSide : public GameObjectClientSide {
protected:



public:
};




class Enemy1ServerSide : public EnemyServerSide {
public:
	Enemy1ServerSide(MultiMap *map) : EnemyServerSide(map) {
		velocity = Vector2::Right * speed;
	}

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY1; }
};

class Enemy2ServerSide : public EnemyServerSide {
public:
	Enemy2ServerSide(MultiMap *map) : EnemyServerSide(map) { }

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY2; }
};

class Enemy3ServerSide : public EnemyServerSide {
public:
	float activeRadius = 50.0f;		// ŒŸ’m”ÍˆÍ
	Enemy3ServerSide(MultiMap *map) : EnemyServerSide(map) { }

	void Loop(void) override;
	MULTI_OBJECT_TYPE GetType(void) { return MULTI_OBJECT_TYPE::MULTI_ENEMY3; }
};
class Enemy1ClientSide : public EnemyClientSide {
protected:
	float speed = 10.0f;

public:



public:
	Enemy1ClientSide() {
		//texNo = LoadTexture(Asset::textures_.at())
	}
};
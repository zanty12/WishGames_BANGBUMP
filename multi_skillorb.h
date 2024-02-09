#pragma once
#include "asset.h"
#include "multi_object.h"
#include "multi_anim.h"
#include "lib/win_time.h"
#include "multi_se.h"

/*******************************************************
  Server
********************************************************/
class ServerSkillOrb : public GameObjectServerSide {
public:
	int addPoint = 1;
	WIN::Time timer;
	float friction = 0.95f;
	

	ServerSkillOrb() { };
	ServerSkillOrb(int addPoint, Transform transform) : addPoint(addPoint), GameObjectServerSide(transform) { }
	ServerSkillOrb(int addPoint, Transform transform, Vector2 velocity) : addPoint(addPoint), GameObjectServerSide(transform) { this->velocity = velocity; }

	void Initialize(void) override;
	void Loop(void) override;
};
class ServerSkillOrbSmall : public ServerSkillOrb {
public:
	static const int AddPoint = 1;
	ServerSkillOrbSmall(Transform transform) : ServerSkillOrb(AddPoint, transform) { };
	ServerSkillOrbSmall(Transform transform, Vector2 velocity) : ServerSkillOrb(AddPoint, transform, velocity) { };
	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_SKILL_POINT_SMALL; }
};
class ServerSkillOrbMidium : public ServerSkillOrb {
public:
	static const int AddPoint = 3;
	ServerSkillOrbMidium(Transform transform) : ServerSkillOrb(AddPoint, transform) { };
	ServerSkillOrbMidium(Transform transform, Vector2 velocity) : ServerSkillOrb(AddPoint, transform, velocity) { };
	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_SKILL_POINT_MIDIUM; }
};
class ServerSkillOrbBig : public ServerSkillOrb {
public:
	static const int AddPoint = 5;
	ServerSkillOrbBig(Transform transform) : ServerSkillOrb(AddPoint, transform) { };
	ServerSkillOrbBig(Transform transform, Vector2 velocity) : ServerSkillOrb(AddPoint, transform, velocity) { };
	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_SKILL_POINT_BIG; }
};




/*******************************************************
  Client
********************************************************/
class ClientSkillOrb : public GameObjectClientSide {
protected:
	MultiAnimator anim;
	int haveSeNo = LoadSound("data/sound/SE/skillorb.wav");

public:
	ClientSkillOrb(Vector2 scale);

	void Loop(void) override;
	void Release(void) override;
};
class ClientSkillOrbSmall : public ClientSkillOrb {
public:
	ClientSkillOrbSmall();
};
class ClientSkillOrbMidium : public ClientSkillOrb {
public:
	ClientSkillOrbMidium();
};
class ClientSkillOrbBig : public ClientSkillOrb {
public:
	ClientSkillOrbBig();
};

	
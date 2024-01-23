#pragma once
#include "asset.h"
#include "multi_object.h"
#include "lib/win_time.h"

/*******************************************************
  Server
********************************************************/
class ServerSkillOrb : public GameObjectServerSide {
public:
	int addPoint = 1;
	WIN::Time timer;
	

	ServerSkillOrb() { };
	ServerSkillOrb(int addPoint, Transform transform) : addPoint(addPoint), GameObjectServerSide(transform) { }

	void Initialize(void) override;
	void Loop(void) override;
};
class ServerSkillOrbSmall : public ServerSkillOrb {
public:
	static const int AddPoint = 1;
	ServerSkillOrbSmall(Transform transform) : ServerSkillOrb(AddPoint, transform) { };
	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_SKILL_POINT_SMALL; }
};
class ServerSkillOrbMidium : public ServerSkillOrb {
public:
	static const int AddPoint = 2;
	ServerSkillOrbMidium(Transform transform) : ServerSkillOrb(AddPoint, transform) { };
	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_SKILL_POINT_MIDIUM; }
};
class ServerSkillOrbBig : public ServerSkillOrb {
public:
	static const int AddPoint = 3;
	ServerSkillOrbBig(Transform transform) : ServerSkillOrb(AddPoint, transform) { };
	MULTI_OBJECT_TYPE GetType(void) override { return MULTI_OBJECT_TYPE::MULTI_SKILL_POINT_BIG; }
};




/*******************************************************
  Client
********************************************************/
class ClientSkillOrb : public ClientGameObject {
public:
	ClientSkillOrb(Vector2 scale);

	void Loop(void) override;
};
class ClientSkillOrbSmall : public ClientSkillOrb {
public:
	ClientSkillOrbSmall() : ClientSkillOrb(Vector2::One * 50) { };
};
class ClientSkillOrbMidium : public ClientSkillOrb {
public:
	ClientSkillOrbMidium() : ClientSkillOrb(Vector2::One * 60) { };
};
class ClientSkillOrbBig : public ClientSkillOrb {
public:
	ClientSkillOrbBig() : ClientSkillOrb(Vector2::One * 80) { };
};


#pragma once
#include "asset.h"
#include "multi_object.h"

class ServerSkillOrb : public ServerGameObject {
public:


	ServerSkillOrb() { radius = 50.0f; };
	ServerSkillOrb(Transform transform) : ServerGameObject(transform) { radius = 50.0f; }
};

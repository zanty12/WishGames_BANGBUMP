#pragma once
#include "asset.h"
#include "multi_object.h"

class ServerSkillOrb : public ServerGameObject {
public:
	ServerSkillOrb() { };
	ServerSkillOrb(Transform transform) : ServerGameObject(transform) { }

	void Initialize(void) override;

	void Loop(void) override;
};

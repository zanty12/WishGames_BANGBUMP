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

class ClientSkillOrb : public ClientGameObject {
public:
	ClientSkillOrb() { };
	ClientSkillOrb(Transform transform) : ClientGameObject(transform) { 
		texNo = LoadTexture(Asset::textures_.at(textures::skill_orb));
		transform.scale = Vector2::One * 50;
	}

	void Loop(void) override;
};

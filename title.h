#pragma once
#include "sprite.h"
#include "scene.h"

class Title :public Scene
{
public:
	Title();
	~Title();
	void Update()override;
	void Draw()override;
};


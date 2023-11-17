#pragma once
#include "Scene.h"

class Title :public Scene
{
public:
	Title();
	~Title();

	void Update()override {}

	void Draw()override {}
};

Title::Title()
{
}

Title::~Title()
{
}
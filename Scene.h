#pragma once

enum SCENE
{
	SCENE_TITLE,
	SCENE_MENU,
	SCENE_PREP,
	SCENE_GAME,
	
};

class Scene
{

public:

	virtual void Update() = 0;
	virtual void Draw() = 0;
};

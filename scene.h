#pragma once

enum SCENE
{
	SCENE_TITLE,
	SCENE_MENU,
	SCENE_PREP,
	SCENE_GAME,
	SCENE_RESULT,
	
};

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DebugMenu() = 0;
};

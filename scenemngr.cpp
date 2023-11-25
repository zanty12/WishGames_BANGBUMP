#include "scenemngr.h"
#include "game.h"
SceneMngr::SceneMngr(SCENE scene) {
	switch (scene)
	{
	case SCENE_TITLE:
		break;
	case SCENE_MENU:
		break;
	case SCENE_PREP:
		break;
	case SCENE_GAME:
		scene_ = new Game();
		break;
	default:
		break;
	}
}

void SceneMngr::ChangeScene(SCENE scene) {
	delete scene_;
	switch (scene)
	{
	case SCENE_TITLE:
		break;
	case SCENE_MENU:
		break;
	case SCENE_PREP:
		break;
	case SCENE_GAME:
		scene_ = new Game();
		break;
	default:
		break;
	}
}
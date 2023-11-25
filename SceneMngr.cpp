#include "scenemngr.h"
#include "Game.h"
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
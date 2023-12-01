#include "scenemngr.h"
#include "game.h"
#include "title.h"
#include "result.h"

SceneMngr::SceneMngr(SCENE scene) {
	switch (scene)
	{
	case SCENE_TITLE:
		scene_ = new Title(this);
		break;
	case SCENE_MENU:
		break;
	case SCENE_PREP:
		break;
	case SCENE_GAME:
		scene_ = new Game(this);
		break;
	case SCENE_RESULT:
		scene_ = new Result(this);
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
		scene_ = new Title(this);
		break;
	case SCENE_MENU:
		break;
	case SCENE_PREP:
		break;
	case SCENE_GAME:
		scene_ = new Game(this);
		break;
	case SCENE_RESULT:
		scene_ = new Result(this);
		break;
	default:
		break;
	}
}

void SceneMngr::DebugMenu() {
	scene_->DebugMenu();
}
#pragma once
#include "Scene.h"
class SceneMngr
{
private:
	Scene* scene_;

public:
	SceneMngr() = delete;
	SceneMngr(SCENE scene);
	~SceneMngr() {
		delete scene_;
	}
	void Update() {
		scene_->Update();
	}
	void Draw() {
		scene_->Draw();
	}
	void ChangeScene(){}

};

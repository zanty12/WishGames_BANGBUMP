#pragma once
#include "scene.h"
class SceneMngr
{
private:
	Scene* scene_;

public:
	SceneMngr() = delete;
	explicit SceneMngr(SCENE scene);
	~SceneMngr() {
		delete scene_;
	}
	void Update() const{
		scene_->Update();
		
	}
	void Draw() const{
		scene_->Draw();
	}
	void ChangeScene(SCENE scene);

	Scene* GetScene() const{
		return scene_;
	}
	void DebugMenu();
};

#pragma once
#include <string>

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
	void ChangeScene(SCENE scene, const std::string& message);

	Scene* GetScene() const{
		return scene_;
	}
	void DebugMenu();

private:
	void ParseGame(const std::string& message);
};

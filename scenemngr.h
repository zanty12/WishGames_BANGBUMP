#pragma once
#include <string>

#include "scene.h"
#include "sprite.h"
#include "texture.h"

class SceneMngr
{
private:
    Scene* scene_;
    ID3D11Texture2D* savedFrameTexture_;
    bool captured_;

    bool loading_ = false;
    int loading_tex_ = 0;

public:
    SceneMngr() = delete;
    explicit SceneMngr(SCENE scene);

    ~SceneMngr()
    {
        delete scene_;
    }

    void Update() const
    {
        scene_->Update();
    }

    void Draw() const
    {
        if (!loading_)
            scene_->Draw();
        else
        {
            DrawSprite(loading_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2),
                       0.0f, Vector2(1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }

    void ChangeScene(SCENE scene);
    void ChangeScene(SCENE scene, const std::string& message);


    Scene* GetScene() const
    {
        return scene_;
    }

    void DebugMenu();

private:
    void LoadScene(SCENE scene);
    void ParseGame(const std::string& message);
    void CaptureScreen();
};

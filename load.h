#pragma once
#include <future>

#include "sprite.h"
#include "video.h"
extern bool isNowLoad;

class Load
{
private:
    static Video* video_;
    static int loading_tex_;
public:
    Load()
    {
        loading_tex_ = LoadTexture("data/texture/UI/loading.png");
    }
    ~Load()
    {
        delete video_;
    }

    static void LoadStart()
    {
        NewVideo();
        isNowLoad = true;
        /*std::future<void> load = std::async(std::launch::async, []() {
            while (isNowLoad)
            {
                Graphical::Clear(Color::White);
                Update();
                Draw();
                Graphical::Present();
            }
        });*/
    }

    static void LoadStop()
    {
        isNowLoad = false;
    }

    static void Update(void)
    {
        Time::Update();
        video_->Update();
    }

    static void Draw()
    {
        Graphical::Clear(Color::White);
        /*DebugUI::BeginDraw();
        video_->Draw();
        ImGui::Begin("test");
        ImGui::Text("Loading...");
        ImGui::End();
        DebugUI::EndDraw();*/
        video_->DrawAsResource();
        /*const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
        const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
        DrawSprite(loading_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2),
                   0.0f, Vector2(1920.0f * scale_x, 1080.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));*/
        Graphical::Present();
    }
private:
    static void NewVideo()
    {
        video_ = new Video("data/video/LoadScreen.mp4");
        video_->SetLoop(true);
        video_->SetWindowPos(Vector2(Graphical::GetWidth() /2 , Graphical::GetHeight() / 2));
        video_->SetSize(Vector2(Graphical::GetWidth(), Graphical::GetHeight()));
    }
};
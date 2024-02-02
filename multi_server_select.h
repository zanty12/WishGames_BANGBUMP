#pragma once
#include "multiplay.h"
#include "sprite.h"
#include "texture.h"
#include "lib/ImGui/imgui.h"

class Multi_Server_Select : public Scene
{
private:
    int bg_tex_;
    char ip_[16] = "";
    bool connect_ = false;
    bool start_ = false;
    SceneMngr* scene_mngr_;

public:
    Multi_Server_Select(SceneMngr* scene_mngr) : scene_mngr_(scene_mngr)
    {
        bg_tex_ = LoadTexture("data/texture/UI/multi_server_select.png");
    }

    ~Multi_Server_Select() override = default;

    void Update() override
    {
        if(connect_ && !start_)
        {
            //stop scenemngr and go to multiplay client
            MultiPlayClient client;
            std::string ip = ip_;
            client.Register(ip);
            MSG msg;
            while(!client.isFinish)
            {
                // メッセージ
                if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                {
                    if (msg.message == WM_QUIT)
                    {
                        break;
                    }
                    else
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                }
            }
            client.Unregister();
            start_ = true;
        }
        if(start_)
            scene_mngr_->ChangeScene(SCENE_MENU);
    }

    void Draw() override
    {
        DrawSprite(bg_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2), 0,
                   Vector2(Graphical::GetWidth(), Graphical::GetHeight()), Color::White);
        ImGui::Begin(u8"接続");
        ImGui::InputText("IP", ip_, 16);
        if (ImGui::Button("Link Start"))
        {
            //stop scenemngr and go to multiplay client
            connect_ = true;
        }
        ImGui::End();
    }
    void DebugMenu() override{}
};

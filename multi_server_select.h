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

    ImFont* font_;

public:
    Multi_Server_Select(SceneMngr* scene_mngr) : scene_mngr_(scene_mngr)
    {
        bg_tex_ = LoadTexture("data/texture/UI/multi_server_select.png");
        font_ = ImGui::GetIO().Fonts->Fonts[1];
    }

    ~Multi_Server_Select() override = default;

    void Update() override
    {
        if (connect_ && !start_)
        {
            //stop scenemngr and go to multiplay client
            MultiPlayClient client;
            std::string ip = ip_;
            client.Register(ip);
            MSG msg;
            while (!client.isFinish)
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
        if (start_)
            scene_mngr_->ChangeScene(SCENE_MENU);
        if (Input::GetKeyDown(0, Input::A))
        {
            connect_ = true;
        }
        if (Input::GetKeyDown(0, Input::B))
        {
            scene_mngr_->ChangeScene(SCENE_MENU);
        }
    }

    void Draw() override
    {
        //background
        DrawSprite(bg_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2), 0,
                   Vector2(Graphical::GetWidth(), Graphical::GetHeight()), Color::White);
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoResize;
        bool open = true;
        float textbox_width = 140 * 7.7;
        ImGui::SetNextWindowPos(ImVec2((Graphical::GetWidth() - textbox_width) / 2, Graphical::GetHeight() / 2 - 100));
        //ImGui::SetNextWindowSize(ImVec2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 4));
        // Draw the texture with ImGui
        ImGui::PushFont(font_);
        ImGui::Begin(u8"接続", &open, window_flags);
        ImGui::PushItemWidth(textbox_width);
        ImGui::InputText("##", ip_, 16);
        ImGui::PopItemWidth();
        /*if (ImGui::Button("Link Start"))
        {
            //stop scenemngr and go to multiplay client
            connect_ = true;
        }*/
        //ImGui::GetFont()->Scale /=2;
        ImGui::PopFont();
        ImGui::End();
    }

    void DebugMenu() override
    {
    }
};

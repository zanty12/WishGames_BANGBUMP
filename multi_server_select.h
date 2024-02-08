#pragma once
#include "multiplay.h"
#include "sound.h"
#include "sprite.h"
#include "texture.h"
#include "lib/ImGui/imgui.h"

class Multi_Server_Select : public Scene
{
private:
    char ip_[16] = "";
    bool connect_ = false;
    bool start_ = false;
    bool first_update_ = true;
    SceneMngr* scene_mngr_;
    Video* bg_video_ = nullptr;
    //texture
    int ip_text_tex_;
    int ip_box_tex_;
    int scene_tex_;
    int confirm_tex_;
    int return_tex_;
    int b_tex_;
    //sound
    int bgm_;
    int confirm_se_;
    int back_se_;

    ImFont* font_;

public:
    Multi_Server_Select(SceneMngr* scene_mngr) : scene_mngr_(scene_mngr)
    {
        ip_text_tex_ = LoadTexture("data/texture/UI/serverselect/IP_text.png");
        ip_box_tex_ = LoadTexture("data/texture/UI/serverselect/IP_No.png");
        scene_tex_ = LoadTexture("data/texture/UI/serverselect/UI_serverselect.png");
        bg_video_ = new Video("data/video/BG_serverselect.mp4");
        confirm_tex_ = LoadTexture("data/texture/UI/serverselect/join.png");
        return_tex_ = LoadTexture("data/texture/UI/back.png");
        b_tex_ = LoadTexture("data/texture/UI/B.png");
        bg_video_->SetLoop(true);
        bg_video_->SetWindowPos(Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2));
        bg_video_->SetSize(Vector2(Graphical::GetWidth(), Graphical::GetHeight()));
        bgm_ = LoadSound("data/sound/bgm/serverselect.wav");
        confirm_se_ = LoadSound("data/sound/se/confirm.wav");
        back_se_ = LoadSound("data/sound/se/back.wav");
        font_ = ImGui::GetIO().Fonts->Fonts[1];
    }

    ~Multi_Server_Select() override
    {
        StopSound(bgm_);
    }


    void Update() override
    {
        if(first_update_)
        {
            SetVolume(bgm_, 0.4f);
            PlaySound(bgm_,-1);
            first_update_ = false;
        }
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
        if (Input::GetKeyDown(0, Input::A) || GetKeyState(VK_RETURN) & 0x8000)
        {
            connect_ = true;
            PlaySound(confirm_se_, 0);
            StopSound(bgm_);
        }
        if (Input::GetKeyDown(0, Input::B))
        {
            PlaySound(back_se_, 0);
            scene_mngr_->ChangeScene(SCENE_MENU);
        }
        if (!start_ && !connect_)
            bg_video_->Update();
    }

    void Draw() override
    {
        //background
        bg_video_->DrawAsResource();
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoResize;
        bool open = true;
        float textbox_width = 1070; //arbitrary value adjusted to fit all characters into a single line
        DrawSprite(ip_text_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() * 2 / 3), 0.0f,
                   Vector2(1200, 300), Color(1.0f, 1.0f, 1.0f, 1.0f));
        DrawSprite(ip_box_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2), 0.0f,
                   Vector2(1200, 180), Color(1.0f, 1.0f, 1.0f, 1.0f));
        DrawSprite(scene_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() - 70), 0.0f,
                   // arbirary value of the texture
                   Vector2(650, 650), Color(1.0f, 1.0f, 1.0f, 1.0f));\
        if (ip_[0] != '\0')
            DrawSprite(confirm_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 3 - 85.0f), 0.0f,
                       Vector2(350, 350), Color(1.0f, 1.0f, 1.0f, AlphaAnimation()));
        //return
        DrawSprite(return_tex_, Vector2(((Graphical::GetWidth() - 100)), 80), 0.0f,
                   Vector2(80.0f, 80.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
        DrawSprite(b_tex_, Vector2(((Graphical::GetWidth() - 160)), 80), 0.0f,
                   Vector2(100.0f, 100.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::SetNextWindowPos(ImVec2((Graphical::GetWidth() - textbox_width) / 2, Graphical::GetHeight() / 2 - 100));
        //arbitrary value to fit text in to the textbox texture
        // Draw the texture with ImGui
        ImGui::PushFont(font_);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
        ImGui::Begin(u8"接続", &open, window_flags);
        ImGui::PushItemWidth(textbox_width);
        if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
            ImGui::SetKeyboardFocusHere(0);
        ImGui::InputText("##", ip_, IM_ARRAYSIZE(ip_));
        ImGui::PopItemWidth();
        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::End();
    }

    void DebugMenu() override
    {
    }

    float AlphaAnimation()
    {
        static float time;
        time += Time::GetDeltaTime() * 1.5f;
        return (sinf(time) + 1.0f) / 2.0f;
    }
};

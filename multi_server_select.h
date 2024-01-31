#pragma once
#include "multiplay.h"
#include "sprite.h"
#include "texture.h"
#include "lib/ImGui/imgui.h"

class Multi_Server_Select
{
private:
    int bg_tex_;
    char* ip_ = nullptr;
    bool connect_ = false;

public:
    Multi_Server_Select()
    {
        bg_tex_ = LoadTexture("data/texture/UI/multi_server_select.png");
        ip_ = new char[16];
    }

    ~Multi_Server_Select() = default;

    void Update()
    {
        if(connect_)
        {
            //stop scenemngr and go to multiplay client
            MultiPlayClient client;
            std::string ip = ip_;
            client.Register(ip);
            MSG msg;
            while(!client.isFinish())
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
        }
    }

    void Draw()
    {
        DrawSprite(bg_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2), 0,
                   Vector2(Graphical::GetWidth(), Graphical::GetHeight()), Color::White);
        ImGui::Begin("鯖接続");
        ImGui::InputText("IP", ip_, 16);
        if (ImGui::Button("Link Start"))
        {
            //stop scenemngr and go to multiplay client
        }
        ImGui::End();
    }
};

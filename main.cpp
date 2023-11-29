#include "graphical.h"
#include "sprite.h"
#include <iostream>
#include "DebugUI.h"
#include "xinput.h"
#include "mapmngr.h"
#include "player.h"
#include "scenemngr.h"
#include "time.h"

bool debug_mode = false;

int main()
{
    Graphical::Initialize(1600, 900);
    DebugUI::Initialize();
    MSG msg;
    int texNo = LoadTexture("player.jpg");
    SceneMngr* scene_mngr = new SceneMngr(SCENE_GAME);
    Time::Initialize();
    while (true)
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
        else{
            Graphical::Clear(Color(1, 1, 1, 1) * 0.5f);
            //デバッグモード
            { if (GetKeyState(VK_F1) & 0x8000)
                debug_mode = !debug_mode;
                DebugUI::BeginDraw();
                if (debug_mode)
                {
                    bool show_demo_window = true;
                    ImGui::ShowDemoWindow(&show_demo_window);
                    ImGuiIO& io = ImGui::GetIO();
                    ImGui::Begin("Debug");
                    ImGui::Text("FPS:%.1f", io.Framerate);
                    ImGui::End();
                    ImGui::Begin("Hello, world!");
                    //ImGui::Text(u8"テキスト");
                    if (ImGui::Button(u8"ボタン"))
                    {
                        std::cout << "ボタン押したよ" << std::endl;
                    }
                    ImGui::End();
                }
            }

            //DrawSprite(texNo, Vector2(100, 100), 0, Vector2(100, 100), Color(1, 1, 1));
            Time::Update();
            scene_mngr->Update();
            scene_mngr->Draw();
            std::cout << Time::GetDeltaTime() << std::endl;

            DebugUI::EndDraw();
            Graphical::Present();
        }
    }

    Graphical::Release();
    DebugUI::Release();
    Time::Release();


    std::cout << "Hello World!\n"; //基本
}

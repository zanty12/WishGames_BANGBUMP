#include "graphical.h"
#include "sprite.h"
#include <iostream>
#include <thread>

#include "DebugUI.h"
#include "xinput.h"
#include "mapmngr.h"
#include "scenemngr.h"
#include "time.h"
#include "video.h"

bool debug_mode = true;

int main()
{
    Graphical::Initialize(1600, 900);
    DebugUI::Initialize();
    MSG msg;
    SceneMngr* scene_mngr = new SceneMngr(SCENE_TITLE);
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
                    //bool show_demo_window = true;
                    //ImGui::ShowDemoWindow(&show_demo_window);
                    ImGuiIO& io = ImGui::GetIO();
                    ImGui::Begin("Main System");
                    ImGui::Text("FPS:%.1f", io.Framerate);


                    //test controller
                    ImGui::Text(u8"コントローラー");
                    ImGui::Text("Left Stick");
                    ImGui::Text("X:%.2f, Y:%.2f", Input::GetStickLeft(0).x,Input::GetStickLeft(0).y);
                    ImGui::Text("Right Stick");
                    ImGui::Text("X:%.2f, Y:%.2f", Input::GetStickRight(0).x, Input::GetStickRight(0).y);

                    //Time
                    ImGui::Text("DeltaTime:%.4f", Time::GetDeltaTime());
                    ImGui::End();

                    scene_mngr->DebugMenu();
                }
            }
            Input::Update();

            Time::Update();

            scene_mngr->Update();
            scene_mngr->Draw();
            DebugUI::EndDraw();
            static float rrrr = 0.0f;
            rrrr += 0.01f;
            std::cout << rrrr << std::endl;
            DrawUICircle(0, Vector2(500, 500), 0, Vector2(500, 500), Color::White, rrrr);

            Graphical::Present();
        }
    }

    Graphical::Release();
    DebugUI::Release();
    Time::Release();

    std::cout << "Hello World!\n"; //基本
}

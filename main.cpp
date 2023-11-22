#include "graphical.h"
#include "sprite.h"
#include <iostream>
#include "DebugUI.h"
#include "xinput.h"
#include "mapmngr.h"

bool debug_mode = false;

int main()
{
    Graphical::Initialize(1600, 900);
    DebugUI::Initialize();
    MSG msg;
    int texNo = LoadTexture("player.jpg");
    MapMngr map_mngr("data/MAP/mariomaker.csv");
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
        else
        {
            if (GetKeyState(VK_F1) & 0x8000)
                debug_mode = !debug_mode;
            Graphical::Clear(Color(1, 1, 1, 1) * 0.5f);
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

            //DrawSprite(texNo, Vector2(100, 100), 0, Vector2(100, 100), Color(1, 1, 1));
            map_mngr.Draw();


            DebugUI::EndDraw();
            Graphical::Present();
        }
    }

    Graphical::Release();
    DebugUI::Release();


    std::cout << "Hello World!\n"; //基本
}

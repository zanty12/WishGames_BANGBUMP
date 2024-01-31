#include "graphical.h"
#include "sprite.h"
#include <iostream>
#include <thread>

#include "DebugUI.h"
#include "xinput.h"
#include "multiplay.h"
#include "time.h"
#include "sound.h"

bool debug_mode = true;
//#define SERVER

int main()
{
    MSG msg;
    Graphical::Initialize(1600, 900);
    DebugUI::Initialize();
    Time::Initialize();
    Text::CreateResources();
    srand(time(NULL));

#ifdef SERVER
    MultiPlayServer server;
    server.OpenTerminal();

#else
    WIN::Window window = Graphical::GetHwnd();
    const HWND hWnd = window.GetHwnd();
    InitSound(hWnd);
    SceneMngr* scene_mngr = new SceneMngr(SCENE_TITLE);
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
            //WTF?
            Graphical::Clear(Color(1, 1, 1, 1) * 0.5f);

            Input::Update();

            Time::Update();

            scene_mngr->Update();
            DebugUI::BeginDraw();
            //デバッグモード
            { if (GetKeyState(VK_F1) & 0x8000)
                debug_mode = !debug_mode;

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
            Text::TextStart();
            scene_mngr->Draw();
            Text::TextEnd();
            DebugUI::EndDraw();
            Graphical::Present();
        }
    }
    //client.Unregister();
    UninitSound();
    delete scene_mngr;
#endif

    Time::Release();
    Text::DiscardResources();
    DebugUI::Release();
    Graphical::Release();

    std::cout << "END\n"; //基本
}

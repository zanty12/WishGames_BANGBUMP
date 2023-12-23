#include "collider.h"
#include "graphical.h"
#include "sprite.h"
#include <iostream>
#include "DebugUI.h"
#include "xinput.h"
#include "mapmngr.h"
#include "scenemngr.h"
#include "time.h"
#include "multiplay.h"

bool debug_mode = true;
#include <thread>


int main()
{
    int mode = 0;
    std::cin >> mode;

    Graphical::Initialize(1600, 900);
    DebugUI::Initialize();
    Time::Initialize();

    if (mode == 0) {
        MultiPlayServer server;
        server.OpenTerminal(); 
    }
    else {
        MSG msg;
        //MultiPlayClient client;
        //client.Register();

        //SceneMngr* scene_mngr = new SceneMngr(SCENE_TITLE);
        //auto func = [&]() {
        //    DWORD dwStartTime = timeGetTime();
        //    DWORD dwCurrentTime = timeGetTime();
        //    DWORD dwOnceFrameTime = 1000 / 60;

        //    while (true) {
        //        dwCurrentTime = timeGetTime();
        //        if (dwCurrentTime - dwStartTime > dwOnceFrameTime * 2) {
        //            dwStartTime = dwCurrentTime;
        //            Input::Update();
        //            client.SendUpdate();
        //        }
        //        if (GetAsyncKeyState(VK_ESCAPE)) return;
        //    }
        //    };
        //std::thread sendInputFunc(func);

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
            else {

                Graphical::Clear(Color(1, 1, 1, 1) * 0.5f);
                //デバッグモード
                { if (GetKeyState(VK_F1) & 0x8000)
                    debug_mode = !debug_mode;
                DebugUI::BeginDraw();
                if (debug_mode)
                {
                    //bool show_demo_window = true;
                    //ImGui::ShowDemoWindow(&show_demo_window);
                    ImGuiIO &io = ImGui::GetIO();
                    ImGui::Begin("Main System");
                    ImGui::Text("FPS:%.1f", io.Framerate);

                    //test controller
                    ImGui::Text(u8"コントローラー");
                    ImGui::Text("Left Stick");
                    ImGui::Text("X:%.2f, Y:%.2f", Input::GetStickLeft(0).x, Input::GetStickLeft(0).y);
                    ImGui::Text("Right Stick");
                    ImGui::Text("X:%.2f, Y:%.2f", Input::GetStickRight(0).x, Input::GetStickRight(0).y);

                    //Time
                    ImGui::Text("DeltaTime:%.4f", Time::GetDeltaTime());
                    ImGui::End();

                }
                //Time::Update();
                //client.Update();
                Input::Update();
                std::cout << "111 : " << Input::GetStickLeft(0).x << ", " << Input::GetStickLeft(0).y << std::endl;


                if (GetAsyncKeyState(VK_ESCAPE)) {
                    break;
                }

                DebugUI::EndDraw();
                Graphical::Present();
                }
            }
        }
        //client.Unregister();
        //sendInputFunc.join();
        std::cout << "Hello World!\n"; //基本
    }
    Time::Release();
    DebugUI::Release();
    Graphical::Release();
}

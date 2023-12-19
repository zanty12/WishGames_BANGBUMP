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
void SendUpdate(Client &client) {
    while (true) {
        client.SendUpdate();
        if (GetAsyncKeyState(VK_ESCAPE)) return;
    }
}

int main()
{
    int mode = 0;
    std::cin >> mode;

    Graphical::Initialize(1600, 900);
    DebugUI::Initialize();
    Time::Initialize();

    if (mode == 0) {
        MultiServer server;
        server.OpenTerminal();
    }
    else {
        MSG msg;
        Client client;
        client.Register();
        
        //SceneMngr* scene_mngr = new SceneMngr(SCENE_TITLE);
        std::thread sendInputFunc(SendUpdate, client);

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

                    //scene_mngr->DebugMenu();
                }
                Input::Update();
                Time::Update();
                client.Update();
                //scene_mngr->Update();
                //scene_mngr->Draw();

                if (GetAsyncKeyState(VK_ESCAPE)) {
                    break;
                }

                DebugUI::EndDraw();
                Graphical::Present();
                }
            }
        }
        client.Unregister();
        sendInputFunc.join();
        std::cout << "Hello World!\n"; //基本
    }
    Time::Release();
    DebugUI::Release();
    Graphical::Release();
}

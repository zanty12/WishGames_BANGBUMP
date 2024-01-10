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

        //while (!client.isFinish)
        while (true)
        {
            // メッセージ
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    //client.isFinish = true;
                    break;
                }
                else
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            Graphical::Clear(Color(1.0f, 1.0f, 1.0f, 1.0f) * 0.5f);
            Vector2 pos = Vector2(100, 100);
            Vector2 scl = Vector2(100, 100);
            float rot = 0.0f;

            DrawSpriteLeftTop(LoadTexture("data/texture/UI/number.png"), pos, rot, scl, Color::White, Vector2(1, 1), Vector2(0.333f, 0.5f));
            Graphical::Present();
        }
        //client.Unregister();
        std::cout << "Hello World!\n"; //基本
    }
    Time::Release();
    DebugUI::Release();
    Graphical::Release();
}

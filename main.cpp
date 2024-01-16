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
    Text::CreateResources();

    if (mode == 0) {
        MultiPlayServer server;
        server.OpenTerminal(); 
    }
    else {
        MSG msg;
        MultiPlayClient client;
        client.Register();

        while (!client.isFinish)
        {
            // メッセージ
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    client.isFinish = true;
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
        std::cout << "Hello World!\n"; //基本
    }
    Time::Release();
    DebugUI::Release();
    Text::DiscardResources();
    Graphical::Release();
}

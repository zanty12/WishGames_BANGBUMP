#include "graphical.h"
#include "sprite.h"
#include <iostream>
#include <thread>

#include "DebugUI.h"
#include "xinput.h"
#include "multiplay.h"
#include "mapmngr.h"
#include "scenemngr.h"
#include "text.h"
#include "time.h"
#include "video.h"

bool debug_mode = true;

int main()
{
    int mode = 0;
    std::cin >> mode;

    MSG msg;
    Graphical::Initialize(1600, 900);
    Graphical::Clear(Color(1, 1, 1, 1) * 0.5f);
    DebugUI::Initialize();
    Time::Initialize();
    Text::CreateResources();
    srand(time(NULL));

    if (mode == 0) {
        MultiPlayServer server;
        server.OpenTerminal();
    }
    else {
        MultiPlayClient client;
        client.Register();
        while (!client.isFinish) {
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
    }

    Time::Release();
    Text::DiscardResources();
    DebugUI::Release();
    Graphical::Release();

    std::cout << "Hello World!\n"; //基本
}

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

    Graphical::Initialize(1600, 900);
    DebugUI::Initialize();
    MSG msg;
    Time::Initialize();
    HRESULT result = Text::CreateResources();
    if (FAILED(result))
    {
        return 0;
    }
    srand(time(NULL));

    if (mode == 0) {
        MultiPlayServer server;
        server.OpenTerminal();
    }
    else {
        MultiPlayClient client;
        client.Register();
        while (true) {
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

    Graphical::Release();
    DebugUI::Release();
    Time::Release();
    Text::DiscardResources();

    std::cout << "Hello World!\n"; //基本
}

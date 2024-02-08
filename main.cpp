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
    int mode = -1;
    std::cin >> mode;

    Time::Initialize();
    srand(time(NULL));

#ifdef SERVER
    MultiPlayServer server;
    server.OpenTerminal();
#else

    if (mode == 0) {
        Graphical::Initialize(1920 * 0.5f, 1080 * 0.5f, TRUE);
        DebugUI::Initialize();
        Text::CreateResources();
        WIN::Window window = Graphical::GetHwnd();
        const HWND hWnd = window.GetHwnd();
        InitSound(hWnd);
        MultiPlayServer server;
        server.OpenTerminal();
    }
    else {
        MSG msg;
        Graphical::Initialize(1920, 1080, FALSE);
        DebugUI::Initialize();
        Text::CreateResources();
        WIN::Window window = Graphical::GetHwnd();
        const HWND hWnd = window.GetHwnd();
        InitSound(hWnd);



        MultiPlayClient client;
        client.Register();

        while (!client.isFinish)
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
        }

        client.Unregister();
    }
    UninitSound();
    Text::DiscardResources();
    DebugUI::Release();
    Graphical::Release();
#endif

    Time::Release();
    std::cout << "END\n"; //基本
}
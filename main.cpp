#include "graphical.h"
#include "sprite.h"
#include <iostream>
#include <thread>

//#include "DebugUI.h"
#include "xinput.h"
#include "multiplay.h"
//#include "mapmngr.h"
//#include "scenemngr.h"
//#include "text.h"
#include "time.h"
//#include "video.h"
#include "multi_map.h"

bool debug_mode = true;


int main()
{
    int mode = 0;
    std::cin >> mode;

    MSG msg;
    Graphical::Initialize(1600, 900);
    Graphical::Clear(Color(1, 1, 1, 1) * 0.5f);
    //DebugUI::Initialize();
    Time::Initialize();
    //Text::CreateResources();
    srand(time(NULL));

    MultiMap map;
    map.Initialize();
    map.Load("data/map/MultiPlay_Map1.csv");

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
                    break;
                }
                else
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

            //static float x = 0;
            //static float y = 0;
            //x += (GetAsyncKeyState('A') - GetAsyncKeyState('D')) * 0.001f;
            //y += (GetAsyncKeyState('S') - GetAsyncKeyState('W')) * 0.001f;

            //Graphical::Clear(Color::White);
            //map.Draw(Vector2(x, y));
            //Graphical::Present();
        }
        client.Unregister();
    }

    Time::Release();
    //Text::DiscardResources();
    //DebugUI::Release();
    Graphical::Release();

    std::cout << "Hello World!\n"; //基本
}

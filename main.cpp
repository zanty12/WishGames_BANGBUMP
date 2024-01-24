#include "graphical.h"
#include "sprite.h"
#include <iostream>
#include <thread>

//#include "DebugUI.h"
#include "xinput.h"
#include "multiplay.h"
#include "asset.h"
//#include "mapmngr.h"
//#include "scenemngr.h"
//#include "text.h"
#include "time.h"
//#include "video.h"
#include "multi_map.h"

bool debug_mode = true;


int main() {
    int mode = 0;
    std::cin >> mode;

    MSG msg;
    Graphical::Initialize(1600, 900);
    DebugUI::Initialize();
    Time::Initialize();
    Text::CreateResources();
    srand(time(NULL));

    //MultiMap map;
    //ServerPlayer player;
    //map.Initialize();
    //map.Load("data/map/MultiPlay_Map1.csv");
    //player.transform.position = *map.startPosition.begin() * map.cellSize;

    if (mode == 0) {
        MultiPlayServer server;
        server.OpenTerminal();
    }
    else {
        MultiPlayClient client;
        client.Register();
        //int texNo = LoadTexture(Asset::textures_.at(textures::player));
        while (!client.isFinish) {
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


            //Input::Update();
            //Graphical::Clear(Color::White);

            //map.Draw(Vector2::Zero);
            //player.velocity = Input::GetStickLeft(0) * 10;
            //Vector2 normal;
            //int id = map.Collision(player.transform.position, 10, &normal);
            //player.transform.position += player.velocity;
            //DrawSprite(texNo, player.transform.position, 0.0, player.transform.scale * 100, Color::White);

            //
            //Graphical::Present();
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
    Text::DiscardResources();
    DebugUI::Release();
    Graphical::Release();

    std::cout << "Hello World!\n"; //基本
}

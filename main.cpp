#include "graphical.h"
#include "sprite.h"
#include <iostream>
#include <thread>

#include "DebugUI.h"
#include "xinput.h"
#include "mapmngr.h"
#include "scenemngr.h"
#include "text.h"
#include "time.h"
#include "video.h"

bool debug_mode = true;

int main()
{
    Graphical::Initialize(1600, 900);
    DebugUI::Initialize();
    MSG msg;
    Time::Initialize();
    HRESULT result = Text::CreateResources();
    if (FAILED(result))
    {
        return 0;
    }
    SceneMngr* scene_mngr = new SceneMngr(SCENE_TITLE);
    srand(time(NULL));
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
        else{
            //WTF?
            //Graphical::Clear(Color(1, 1, 1, 1) * 0.5f);

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

            using namespace PHYSICS;
            static float x = 100.0f;
            static float y = 100.0f;
            static float r = 0.0f;
            x += Input::GetStickLeft(0).x;
            y += Input::GetStickLeft(0).y;
            x += Input::GetKey(0, Input::Right) - Input::GetKey(0, Input::Left);
            y += Input::GetKey(0, Input::Up) - Input::GetKey(0, Input::Down);
            r += Input::GetKey(0, Input::R) * 0.01f;
            r -= Input::GetKey(0, Input::L) * 0.01f;

            Vertex1 v1(Vector2(100, 100), 100);
            Vertex4 v4(Vector2(x,y), r, Vector2(100,100));
            Color color = Collider2D::TouchNew(v1, VertexN(v4.v, 4)) ? Color::Red : Color::Green;
            DrawCollider(v1, color, Vector2::Zero);
            DrawCollider(v4, color, Vector2::Zero);

            Graphical::Present();
        }
    }

    Graphical::Release();
    DebugUI::Release();
    Time::Release();
    Text::DiscardResources();

    std::cout << "Hello World!\n"; //基本
}

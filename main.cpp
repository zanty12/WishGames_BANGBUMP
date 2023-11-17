#include "graphical.h"
#include "sprite.h"
#include <iostream>
#include "DebugUI.h"

#include "lib/winlib.h"






int main()
{
	Graphical::Initialize(960, 540);
#ifdef _DEBUG
	DebugUI::Initialize();
#endif
	MSG msg;
	int texNo = LoadTexture("player.jpg");

	while (true) {
		// メッセージ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			Graphical::Clear(Color(1, 1, 1, 1) * 0.5f);
#ifdef _DEBUG
			DebugUI::BeginDraw();
			{
				ImGui::Begin("Hello, world!");
				//ImGui::Text(u8"テキスト");
				if (ImGui::Button(u8"ボタン"))
				{
					std::cout << "ボタン押したよ" << std::endl;
				}

				ImGui::End();
			}
#endif
			DrawSprite(texNo, Vector2(100, 100), 0, Vector2(100, 100), Color(1, 1, 1));

#ifdef _DEBUG
			DebugUI::EndDraw();
#endif
			Graphical::Present();
		}
	}

	Graphical::Release();
#ifdef DEBUG
	DebugUI::Release();
#endif // DEBUG

	std::cout << "Hello World!\n";//基本
}
#include "graphical.h"
#include "sprite.h"
#include <iostream>

int main()
{
	Graphical::Initialize(1920, 1080);
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
			DrawSprite(texNo, Vector2(0, 0), 0, Vector2(100, 100), Color(1, 1, 1));
			Graphical::Present();
		}
	}


	Graphical::Release();
	std::cout << "Hello World!\n";//基本
}



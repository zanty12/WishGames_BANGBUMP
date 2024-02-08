#include "load.h"
#include "graphical.h"
#include "multi_anim.h"

bool isNowLoad = false;

void Load(void) {
	MultiAnimator anim = MultiAnimator(LoadTexture("data/texture/UI/UI_NOWLOADLING.png"), 5, 12, 0, 59);
	float width = Graphical::GetWidth();
	float height = Graphical::GetHeight();
	float scale = 100.0f;
	isNowLoad = true;

	while (isNowLoad) {
		Graphical::Clear(Color::Black);
		anim.Draw(Vector2(width - scale * 0.5f, scale * 0.5f), 0.0f, Vector2(scale, scale), Color::White);
		Graphical::Present();
	}

	return;
}

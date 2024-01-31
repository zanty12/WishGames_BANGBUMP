#include "light_effect.h"
#include "lib/math.h"
#include "graphical.h"
#include "sprite.h"

void LightEffect::Draw(Vector2 offset) {
	float curTime = timer.GetNowTime() * 0.001f;

	// エフェクトを生成
	if (curTime <= spawnTime) {
		float halfWidth = Graphical::GetWidth() * 0.5f;
		float halfHeight = Graphical::GetHeight() * 0.5f;
		Light light;
		light.position.x = MATH::Rand(-halfWidth, halfWidth);
		light.position.y = MATH::Rand(-halfHeight, halfHeight);
		light.rad = MATH::Rand(0.0f, MATH::PI * 2.0f);
		light.timer.Start();
		lights.push_back(light);
		timer.Start();
	}

	for (auto iterator = lights.begin(); iterator != lights.end();) {
		auto &light = *iterator;
		float nowTime = light.timer.GetNowTime() * 0.001f;
		// 削除なら
		if (lifeTime <= light.timer.GetNowTime() * 0.001f) {
			auto delIterator = iterator;
			iterator++;
			lights.erase(delIterator);
			continue;
		}


		// 回転
		light.rad += light.radSpeed;
		// ベクトル
		Vector2 velocity = Vector2(std::cos(light.rad), std::sin(light.rad));
		// 移動
		light.position += velocity;

		float t = nowTime / lifeTime;
		float a = MATH::Leap(MATH::Leap(0.0f, 1.0f, t), MATH::Leap(1.0f, 0.0f, t), t);
		// 描画
		DrawSprite(texNo, light.position - offset, 0.0f, light.scale, Color(1.0f, 1.0f, 1.0f, a));



		// 次へ
		iterator++;
	}
}

#include "light_effect.h"
#include "lib/math.h"
#include "graphical.h"
#include "sprite.h"

void LightEffect::Draw(Vector2 offset) {
	float curTime = timer.GetNowTime() * 0.001f;

	// �G�t�F�N�g�𐶐�
	if (curTime <= spawnTime) {
		float halfWidth = Graphical::GetWidth() * 0.5f;
		float halfHeight = Graphical::GetHeight() * 0.5f;
		Light light;
		light.position.x = MATH::Rand(-halfWidth, halfWidth);
		light.position.y = MATH::Rand(-halfHeight, halfHeight);
		light.rad = MATH::Rand(0.0f, MATH::PI * 2.0f);
		light.timer.Start();
		lights.push_back(light);
	}

	for (auto iterator = lights.begin(); iterator != lights.end();) {
		auto &light = *iterator;
		// �폜�Ȃ�
		if (lifeTime <= light.timer.GetNowTime() * 0.001f) {
			auto delIterator = iterator;
			iterator++;
			lights.erase(delIterator);
			continue;
		}


		// ��]
		light.rad += light.radSpeed;
		// �x�N�g��
		Vector2 velocity = Vector2(std::cos(light.rad), std::sin(light.rad));
		// �ړ�
		light.position += velocity;
		// �`��
		DrawSprite(texNo, light.position - offset, 0.0f, light.scale, Color::White);



		// ����
		iterator++;
	}
}

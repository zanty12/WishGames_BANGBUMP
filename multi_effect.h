#pragma once
#include <list>
#include "multi_anim.h"
#include "multi_transform.h"

class EffectClientSide {
private:
	struct object {
		Transform transform;
		Color color = Color::White;
		MultiAnimator animator;
	};
	std::list<object> animators;

public:
	void AddEffect(MultiAnimator anim, Vector2 pos, float rot, Vector2 scl, Color col = Color::White) {
		animators.push_back({ Transform(pos, rot, scl), col, anim });
	}

	void Draw(Vector2 offset) {
		for (auto iterator = animators.begin(); iterator != animators.end();) {			
			auto &obj = *iterator;				// オブジェクト			
			auto &anim = obj.animator;			// アニメーター
			auto &transform = obj.transform;	// 座標
			auto &color = obj.color;			// 色
			// アニメーターが終了状態なら削除
			if (anim.IsEnd()) {
				auto delIterator = iterator;
				iterator++;
				animators.erase(delIterator);
				continue;
			}

			// 描画
			anim.Draw(transform.position - offset, transform.rotation, transform.scale, color);

			// 次へ進む
			iterator++;
		}
	}
};
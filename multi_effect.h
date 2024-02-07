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
			auto &obj = *iterator;				// �I�u�W�F�N�g			
			auto &anim = obj.animator;			// �A�j���[�^�[
			auto &transform = obj.transform;	// ���W
			auto &color = obj.color;			// �F
			// �A�j���[�^�[���I����ԂȂ�폜
			if (anim.IsEnd()) {
				auto delIterator = iterator;
				iterator++;
				animators.erase(delIterator);
				continue;
			}

			// �`��
			anim.Draw(transform.position - offset, transform.rotation, transform.scale, color);

			// ���֐i��
			iterator++;
		}
	}
};
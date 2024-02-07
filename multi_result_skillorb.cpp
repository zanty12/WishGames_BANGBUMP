#include "multi_result_skillorb.h"

void ResultSkillOrb::Loop(void) {
	// �^�[�Q�b�g�̌������v�Z
	Vector2 direction = target - transform.position;

	// �x���V�e�B
	velocity += direction * moveAmount;

	// �^�[�Q�b�g�ɋ߂��Ȃ�폜����
	if (direction.DistanceSq() <= destroyDistance * destroyDistance || 0.0f <= direction.y) {
		isDestroy = true;
	}

	// �ړ�
	transform.position += velocity;

	anim.Draw(transform.position, transform.rotation, transform.scale, Color::White);
}

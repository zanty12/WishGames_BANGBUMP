#include <sstream>
#include "multi_anim.h"
#include "texture.h"



void MultiAnimator::Draw(Vector2 pos, float rot, Vector2 scl, Color col, bool isReverseX, bool isReverseY) {
	DWORD deltaTime = timeGetTime() - startTime;
	// �A�j���[�V����������
	if (frameTime <= deltaTime) {
		idx++;

		startTime = timeGetTime();

		// �I�_�Ȃ�n�_�ɂ���
		if (isLoop) { if (loopEnd < idx) idx = loopBegin; }
		// ���[�v���Ȃ��Ȃ�I��
		else if (end < idx) return;
	}

	// UV�l�̌v�Z
	Vector2 uv = ToUV(idx);
	Vector2 uvScale = Vector2(1.0f / width, 1.0f / height);

	// ���]����
	if (isReverseX) {
		uv.x += uvScale.x;
		uvScale.x = -uvScale.x;
	}
	if (isReverseY) {
		uv.y += uvScale.y;
		uvScale.y = -uvScale.y;
	}

	// �`�悷��
	DrawSprite(texNo, pos, rot, scl, col, uv, uvScale);
}

MultiAnimator MultiAnimator::GetPlayerInitialize(int playerIdx, ATTRIBUTE_TYPE move, ATTRIBUTE_TYPE attack) {
	std::stringstream path;
	int width = 5;
	int height = 1;
	int begin = 0;
	int end = 29;

	path << "data/texture/Player/player" << playerIdx + 1 << "_" << attack + 1 << move + 1 << ".png";

	// �t�@�C�A�i�U���j
	if (attack == ATTRIBUTE_TYPE_FIRE) {
		// �E�H�[�^�i�ړ��j
		if (move == ATTRIBUTE_TYPE_DARK) height = 14;
		// �T���_�[�i�ړ��j
		else if (move == ATTRIBUTE_TYPE_THUNDER) height = 9;
		// �E�B���h�i�ړ��j
		else if (move == ATTRIBUTE_TYPE_WIND) height = 8;
	}
	// �E�H�[�^�i�U���j
	else if (attack == ATTRIBUTE_TYPE_DARK) {
		// �t�@�C�A�i�ړ��j
		if (move == ATTRIBUTE_TYPE_FIRE) height = 14;
		// �T���_�[�i�ړ��j
		else if (move == ATTRIBUTE_TYPE_THUNDER) height = 9;
		// �E�B���h�i�ړ��j
		else if (move == ATTRIBUTE_TYPE_WIND) height = 8;
	}
	// �T���_�[�i�U���j
	else if (attack == ATTRIBUTE_TYPE_THUNDER) {
		// �t�@�C�A�i�ړ��j
		if (move == ATTRIBUTE_TYPE_FIRE) height = 15;
		// �E�H�[�^�i�ړ��j
		else if (move == ATTRIBUTE_TYPE_DARK) height = 15;
		// �E�B���h�i�ړ��j
		else if (move == ATTRIBUTE_TYPE_WIND) height = 9;
	}
	// �E�B���h�U���j
	else if (attack == ATTRIBUTE_TYPE_WIND) {
		// �t�@�C�A�i�ړ��j
		if (move == ATTRIBUTE_TYPE_FIRE) height = 14;
		// �E�H�[�^�i�ړ��j
		else if (move == ATTRIBUTE_TYPE_DARK) height = 14;
		// �T���_�[�i�ړ��j
		else if (move == ATTRIBUTE_TYPE_THUNDER) height = 9;
	}


	return MultiAnimator(LoadTexture(path.str()), width, height, begin, end, true, begin, end);
}

void MultiAnimator::GetPlayer(MULTI_ANIMATION_TYPE animType, ATTRIBUTE_TYPE move, ATTRIBUTE_TYPE attack, MultiAnimator* anim) {
	if (anim == nullptr) return;
	// IDLE
	if (animType == ANIMATION_TYPE_IDEL) {
		anim->begin = 0;
		anim->end = 29;
		anim->loopBegin = 0;
		anim->loopEnd = 29;
		anim->isLoop = true;
		anim->idx = anim->begin;
	}


	int idleIdxWidth = 30;		// �ҋ@�Ɏg���C���f�b�N�X�̕�
	int moveIdxWidth = -1;		// �ړ��Ɏg���C���f�b�N�X�̕�
	int attackIdxWidth = -1;	// �U���Ɏg���C���f�b�N�X�̕�
	switch (move) {
	case ATTRIBUTE_TYPE_FIRE: moveIdxWidth = 30; break;
	case ATTRIBUTE_TYPE_DARK: moveIdxWidth = 30; break;
	case ATTRIBUTE_TYPE_THUNDER: moveIdxWidth = 4; break;
	case ATTRIBUTE_TYPE_WIND: moveIdxWidth = 1; break;
	}
	switch (attack) {
	case ATTRIBUTE_TYPE_FIRE: attackIdxWidth = 9; break;
	case ATTRIBUTE_TYPE_DARK: attackIdxWidth = 9; break;
	case ATTRIBUTE_TYPE_THUNDER: attackIdxWidth = 9; break;
	case ATTRIBUTE_TYPE_WIND: attackIdxWidth = 9; break;
	}

	// ATTACK
	if (animType == ANIMATION_TYPE_ATTACK) {
		int begin = idleIdxWidth;
		anim->begin = begin;
		anim->end = begin + attackIdxWidth - 1;
		anim->loopBegin = begin;
		anim->loopEnd = begin + attackIdxWidth - 1;
		anim->isLoop = true;
		anim->idx = anim->begin;
	}
	// MOVE
	if (animType == ANIMATION_TYPE_MOVE) {
		int begin = idleIdxWidth + attackIdxWidth;
		anim->begin = begin;
		anim->end = begin + moveIdxWidth - 1;
		anim->loopBegin = begin;
		anim->loopEnd = begin + moveIdxWidth - 1;
		anim->isLoop = true;
		anim->idx = anim->begin;
	}
}

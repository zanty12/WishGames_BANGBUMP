#include <sstream>
#include "multi_anim.h"
#include "texture.h"



void MultiAnimator::Draw(Vector2 pos, float rot, Vector2 scl, Color col, bool isReverseX, bool isReverseY) {
	DWORD deltaTime = timeGetTime() - startTime;
	// アニメーションさせる
	if (isAnimation) {
		if (frameTime <= deltaTime) {
			idx++;

			startTime = timeGetTime();
		}

		// 終点なら始点にする
		if (isLoop) { if (loopEnd < idx) idx = loopBegin; }
		// ループしないなら終了
		else if (end < idx) {
			if (!isEndShow)	return;
			else idx = end;
		}
	}

	// UV値の計算
	Vector2 uv = ToUV(idx);
	Vector2 uvScale = Vector2(1.0f / width, 1.0f / height);

	// 反転処理
	if (isReverseX) {
		uv.x += uvScale.x;
		uvScale.x = -uvScale.x;
	}
	if (isReverseY) {
		uv.y += uvScale.y;
		uvScale.y = -uvScale.y;
	}

	// 描画する
	DrawSprite(texNo, pos, rot, scl, col, uv, uvScale);
}

void MultiAnimator::Draw(int idx, Vector2 pos, float rot, Vector2 scl, Color col, bool isReverseX, bool isReverseY) {
	// UV値の計算
	Vector2 uv = ToUV(idx);
	Vector2 uvScale = Vector2(1.0f / width, 1.0f / height);

	// 反転処理
	if (isReverseX) {
		uv.x += uvScale.x;
		uvScale.x = -uvScale.x;
	}
	if (isReverseY) {
		uv.y += uvScale.y;
		uvScale.y = -uvScale.y;
	}

	// 描画する
	DrawSprite(texNo, pos, rot, scl, col, uv, uvScale);
}

MultiAnimator MultiAnimator::GetPlayerInitialize(int playerIdx, ATTRIBUTE_TYPE move, ATTRIBUTE_TYPE attack) {
	std::stringstream path;
	int width = 5;
	int height = 1;
	int begin = 0;
	int end = 29;

	path << "data/texture/Player/player" << playerIdx + 1 << "_" << attack + 1 << move + 1 << ".png";

	// ファイア（攻撃）
	if (attack == ATTRIBUTE_TYPE_FIRE) {
		// ウォータ（移動）
		if (move == ATTRIBUTE_TYPE_DARK) height = 14;
		// サンダー（移動）
		else if (move == ATTRIBUTE_TYPE_THUNDER) height = 9;
		// ウィンド（移動）
		else if (move == ATTRIBUTE_TYPE_WIND) height = 8;
	}
	// ウォータ（攻撃）
	else if (attack == ATTRIBUTE_TYPE_DARK) {
		// ファイア（移動）
		if (move == ATTRIBUTE_TYPE_FIRE) height = 14;
		// サンダー（移動）
		else if (move == ATTRIBUTE_TYPE_THUNDER) height = 9;
		// ウィンド（移動）
		else if (move == ATTRIBUTE_TYPE_WIND) height = 8;
	}
	// サンダー（攻撃）
	else if (attack == ATTRIBUTE_TYPE_THUNDER) {
		// ファイア（移動）
		if (move == ATTRIBUTE_TYPE_FIRE) height = 15;
		// ウォータ（移動）
		else if (move == ATTRIBUTE_TYPE_DARK) height = 15;
		// ウィンド（移動）
		else if (move == ATTRIBUTE_TYPE_WIND) height = 9;
	}
	// ウィンド攻撃）
	else if (attack == ATTRIBUTE_TYPE_WIND) {
		// ファイア（移動）
		if (move == ATTRIBUTE_TYPE_FIRE) height = 14;
		// ウォータ（移動）
		else if (move == ATTRIBUTE_TYPE_DARK) height = 14;
		// サンダー（移動）
		else if (move == ATTRIBUTE_TYPE_THUNDER) height = 9;
	}


	return MultiAnimator(LoadTexture(path.str()), width, height, begin, end, true, begin, end);
}

void MultiAnimator::GetPlayer(int animType, ATTRIBUTE_TYPE move, ATTRIBUTE_TYPE attack, MultiAnimator* anim) {
	if (anim == nullptr) return;
	// IDLE
	if (animType == ANIMATION_TYPE_IDLE) {
		anim->begin = 0;
		anim->end = 29;
		anim->loopBegin = 0;
		anim->loopEnd = 29;
		anim->idx = anim->begin;
		anim->isLoop = true;
		anim->isEndShow = true;
		anim->frameTime = 30 / 1000;
	}


	int idleIdxWidth = 30;		// 待機に使うインデックスの幅
	int moveIdxWidth = -1;		// 移動に使うインデックスの幅
	int attackIdxWidth = -1;	// 攻撃に使うインデックスの幅
	switch (attack) {
	case ATTRIBUTE_TYPE_FIRE: attackIdxWidth = 9; break;
	case ATTRIBUTE_TYPE_DARK: attackIdxWidth = 9; break;
	case ATTRIBUTE_TYPE_THUNDER: attackIdxWidth = 9; break;
	case ATTRIBUTE_TYPE_WIND: attackIdxWidth = 9; break;
	}
	switch (move) {
	case ATTRIBUTE_TYPE_FIRE: moveIdxWidth = 30; break;
	case ATTRIBUTE_TYPE_DARK: moveIdxWidth = 30; break;
	case ATTRIBUTE_TYPE_THUNDER: moveIdxWidth = 1; break;
	case ATTRIBUTE_TYPE_WIND: moveIdxWidth = 1; break;
	}

	// ATTACK
	if (animType & ANIMATION_TYPE_ATTACK) {
		int begin = idleIdxWidth;
		anim->begin = begin;
		anim->end = begin + attackIdxWidth - 1;
		anim->loopBegin = begin;
		anim->loopEnd = begin + attackIdxWidth - 1;
		anim->idx = anim->begin;
		anim->isLoop = true;
		anim->isEndShow = true;
		anim->frameTime = 30 / 1000;
	}
	// MOVE
	if (animType == ANIMATION_TYPE_MOVE) {
		int begin = idleIdxWidth + attackIdxWidth;
		anim->begin = begin;
		anim->end = begin + moveIdxWidth - 1;
		anim->loopBegin = begin;
		anim->loopEnd = begin + moveIdxWidth - 1;
		anim->idx = anim->begin;
		anim->isLoop = true;
		anim->isEndShow = true;
		anim->frameTime = 30 / 1000;
	}
	// ATTACK CHARGE（Thunder）
	if (IsPlayerAnimAttackCharge(animType) && attack == ATTRIBUTE_TYPE_THUNDER) {
		int begin = idleIdxWidth + attackIdxWidth + moveIdxWidth;
		anim->begin = begin;
		anim->end = begin + 4;
		anim->loopBegin = begin;
		anim->loopEnd = begin + 4;
		anim->idx = anim->begin;
		anim->isLoop = true;
		anim->isEndShow = true;
		anim->frameTime = 32 / 1000;
	}
	// MOVE CHARGE（Thunder）
	if (IsPlayerAnimMoveCharge(animType) && move == ATTRIBUTE_TYPE_THUNDER) {
		int begin = idleIdxWidth + attackIdxWidth + moveIdxWidth;
		anim->begin = begin;
		anim->end = begin + 2;
		anim->loopBegin = begin;
		anim->loopEnd = begin + 2;
		anim->idx = anim->begin;
		anim->isLoop = true;
		anim->isEndShow = true;
		anim->frameTime = 32 / 1000;
	}
	// ATTACK（Thunder）
	if (IsPlayerAnimAttack(animType) && attack == ATTRIBUTE_TYPE_THUNDER) {
		anim->isLoop = false;
		anim->isEndShow = true;
		anim->frameTime = 30 / 1000;
	}
}

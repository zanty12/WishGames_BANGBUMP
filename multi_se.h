#pragma once
#include "sound.h"
#include "lib/vector.h"
#include "lib/math.h"

#define SOUND_RANGE (1000.0f)

void PlaySE(int seNo, Vector2 player, Vector2 other) {
	float distanceSq = (player - other).DistanceSq();
	float maxDistanceSq = SOUND_RANGE * SOUND_RANGE;
	if (distanceSq < maxDistanceSq) {
		float rate = distanceSq / maxDistanceSq;
		float volume = MATH::Leap(0.3f, 0.01f, rate);
		PlaySound(seNo, false);
		SetVolume(seNo, volume);
	}
}

void PlaySE(int seNo) {
	float volume = 0.3f;
	PlaySound(seNo, false);
	SetVolume(seNo, volume);
}
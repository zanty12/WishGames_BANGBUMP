#include "multi_se.h"

void PlaySE(int seNo, Vector2 player, Vector2 other) {
	float distanceSq = (player - other).DistanceSq();
	float maxDistanceSq = SOUND_RANGE * SOUND_RANGE;
	if (distanceSq < maxDistanceSq) {
		float rate = distanceSq / maxDistanceSq;
		float volume = MATH::Leap(0.8f, 0.8f, rate);
		SetVolume(seNo, volume);
		PlaySound(seNo, false);
	}
}

void PlaySE(int seNo) {
	float volume = 0.8f;
	SetVolume(seNo, volume);
	PlaySound(seNo, false);
}

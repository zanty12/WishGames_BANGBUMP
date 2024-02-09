#pragma once
#include "sound.h"
#include "lib/vector.h"
#include "lib/math.h"

#define SOUND_RANGE (2000.0f)

void PlaySE(int seNo, Vector2 player, Vector2 other);

void PlaySE(int seNo);

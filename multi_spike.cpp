#include "multi_spike.h"
#include "multi_movable_object.h"

void SpikeServerSide::KnockBack(ServerMovableGameObject *object) {
	object->blownVelocity = blownVelocity * knockbackRate;
}
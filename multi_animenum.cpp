#include "multi_animenum.h"

void SetPlayerAnimIdle(int &type) {
	type = ANIMATION_TYPE_IDLE;
}

void SetPlayerAnimMove(int &type, bool isCharge) {
	SetPlayerAnimNoMove(type);
	if (isCharge) type |= ANIMATION_TYPE_MOVE_CHARGE;
	else type |= ANIMATION_TYPE_MOVE;
}

void SetPlayerAnimNoMove(int &type) {
	type = ~ANIMATION_TYPE_MOVE & type;
}

void SetPlayerAnimAttack(int &type, bool isCharge) {
	SetPlayerAnimNoMove(type);
	if (isCharge) type |= ANIMATION_TYPE_ATTACK_CHARGE;
	else type |= ANIMATION_TYPE_ATTACK;
}

void SetPlayerAnimNoAttack(int &type) {
	type = ~ANIMATION_TYPE_ATTACK & type;
}

bool IsPlayerAnimMove(int type) {
	return (type & ANIMATION_TYPE_MOVE) == ANIMATION_TYPE_MOVE;
}

bool IsPlayerAnimMoveCharge(int type) {
	return (type & ANIMATION_TYPE_MOVE_CHARGE) == ANIMATION_TYPE_MOVE_CHARGE;
}

bool IsPlayerAnimAttack(int type) {
	return (type & ANIMATION_TYPE_ATTACK);
}

bool IsPlayerAnimAttackCharge(int type) {
	return (type & ANIMATION_TYPE_ATTACK_CHARGE) == ANIMATION_TYPE_ATTACK_CHARGE;
}

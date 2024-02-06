#pragma once
enum MULTI_ANIMATION_TYPE {
	ANIMATION_TYPE_IDLE				= 0b00000,
	ANIMATION_TYPE_MOVE				= 0b00110,
	ANIMATION_TYPE_MOVE_CHARGE		= 0b00010,
	ANIMATION_TYPE_ATTACK			= 0b11000,
	ANIMATION_TYPE_ATTACK_CHARGE	= 0b01000,
};

void SetPlayerAnimIdle(int &type);
void SetPlayerAnimMove(int &type, bool isCharge = false);
void SetPlayerAnimNoMove(int &type);
void SetPlayerAnimAttack(int &type, bool isCharge = false);
void SetPlayerAnimNoAttack(int &type);
bool IsPlayerAnimMove(int type);
bool IsPlayerAnimMoveCharge(int type);
bool IsPlayerAnimAttack(int type);
bool IsPlayerAnimAttackCharge(int type);
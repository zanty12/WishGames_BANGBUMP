#pragma once
#include "attribute.h"

class Wind : public Attribute {
private:
	float power_ = 0.0f;
	const float maxPower_ = 5.0f;
	const float rotInputJudgeMin = 1.0f;	// ‚Ü‚í‚·‚ð”»’è‚·‚é
	const float friction_ = 0.95f;			// –€ŽC

public:
	Wind(Player& player) : Attribute(player) { }

	Vector2 Move(void) override;
	void Action(void) override;
};
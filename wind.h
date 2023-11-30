#pragma once
#include "attribute.h"

class Wind : public Attribute {
private:
	float power_ = 0.0f;
	const float maxPower_ = 5.0f;
	const float rotInputFriction = 0.5f;	// �܂킷�����x�̖��C�萔
	const float rotInputJudgeMin = 0.1f;	// �܂킷�𔻒肷��
	const float friction_ = 0.95f;			// ���C

public:
	Wind(Player *player) : Attribute(player) { }

	Vector2 Move(void) override;
	void Action(void) override;
};
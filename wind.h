#pragma once
#include "attribute.h"

class Wind : public Attribute
{
private:
    float power_ = 0.0f;
    //const float maxPower_ = 1000.0f;
    const float rotInputFriction = 0.5f;	// �܂킷�����x�̖��C�萔
    const float rotInputJudgeMin = 0.1f;	// �܂킷�𔻒肷��
    //const float friction_ = 0.95f;			// ���C

    //const float attackRadius = 100.0f;		// �U���͈�
    float prev_y_ = 0.0f;

    bool isDraw = false;
    PHYSICS::Vertex1 attackCollider;
    float previous_time_ = 0.0f;

    //�����̂���const����
    float maxPower_ = 10 * GameObject::SIZE_;
    float friction_ = 0.8f;
    float attackRadius = 100.0f;

public:
    Wind(Player* player) : Attribute(player, ATTRIBUTE_TYPE_WIND)
    {
    }

    ~Wind() override = default;
    bool StickTrigger(Vector2 stick, Vector2 previousStick);
    Vector2 Move(void) override;
    void Action(void) override;
    void Draw(Vector2) override;
    void DebugMenu(void) override;
};

class WindAttack: public MovableObj
{

};

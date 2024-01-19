#pragma once
#include "attribute.h"
#include "playerattack.h"

struct SignalFilter
{
    std::list<int> prev_signals = std::list<int>(10,0);
    int fail_count = 0;
    const int fail_threshold = 3; //allowed failures

    void PassSignal(int signal)
    {
        // Keep the list size to 10
        if (prev_signals.size() == 10)
        {
            prev_signals.pop_front();
        }
        prev_signals.push_back(signal);
    }

    int PredictNext()
    {
        //use prev_signals to predict next signal
        int sum = 0;
        for (int signal : prev_signals)
        {
            sum += signal;
        }
        float average = static_cast<float>(sum) / prev_signals.size();
        if (average >= 0.4)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
};


class WindAttack;

class Wind : public Attribute
{
private:
    float power_ = 0.0f;
    //const float maxPower_ = 1000.0f;
    const float rotInputFriction = 2.0f; // Ç‹ÇÌÇ∑â¡ë¨ìxÇÃñÄéCíËêî
    const float rotInputJudgeMin = 0.03; // Ç‹ÇÌÇ∑ÇîªíËÇ∑ÇÈ
    //const float friction_ = 0.95f;			// ñÄéC

    float prev_y_ = 0.0f;

    WindAttack* attack_ = nullptr;
    float previous_time_ = 0.0f;
    SignalFilter attack_filter_;

    //í≤êÆÇÃÇΩÇﬂconstî≤Ç´
    float maxPower_ = 10 * GameObject::SIZE_;
    float friction_ = 0.8f;

public:
    Wind(Player* player) : Attribute(player, ATTRIBUTE_TYPE_WIND)
    {
    }

    ~Wind() override = default;
    bool StickTrigger(Vector2 stick, Vector2 previousStick);
    Vector2 Move(void) override;
    void Action(void) override;
    void DebugMenu(void) override;
};

class WindAttack : public MovableObj,public PlayerAttack
{
    Wind* parent_;
    Vector2 size_ = Vector2(4 * GameObject::SIZE_, 4* GameObject::SIZE_);
public:
    WindAttack() = delete;
    WindAttack(Wind* parent);
    ~WindAttack() override = default;
    void Update() override{};
};


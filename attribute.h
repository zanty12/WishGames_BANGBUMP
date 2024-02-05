#pragma once
#include "attribute_state.h"
#include "lib/vector.h"
#include "player.h"
#include "attribute_type.h"
#include "lib/win_time.h"


class Player;

class Attribute
{
protected:
    ATTRIBUTE_TYPE attributeType_;
    Player* player_;
    WIN::Time cd_time_;
    WIN::Time attack_life_time_;

    float power_;
    AttributeState* state_ = nullptr;
    AttributeState state_lv_[10] = {};
    static constexpr float server_tick_rate = 60.0f;

public:
    Attribute() = delete;

    Attribute(Player* player, ATTRIBUTE_TYPE attr) : player_(player), attributeType_(attr)
    {
        int lv = 1;
        for (auto& state : state_lv_)
        {
            state = AttributeState(GetAttributeString(attr), lv);
            lv++;
        }
        state_ = state_lv_;
        cd_time_.Start();
    }

    virtual ~Attribute() = default;

    virtual void Move() = 0;

    virtual void Action() = 0;

    virtual void DebugMenu() = 0;

    Player* GetPlayer() { return player_; }

    ATTRIBUTE_TYPE GetAttribute() { return attributeType_; }

    static std::wstring GetAttributeString(ATTRIBUTE_TYPE attr)
    {
        switch (attr)
        {
        case ATTRIBUTE_TYPE::ATTRIBUTE_TYPE_FIRE:
            return L"Fire";
        case ATTRIBUTE_TYPE::ATTRIBUTE_TYPE_WIND:
            return L"Wind";
        case ATTRIBUTE_TYPE::ATTRIBUTE_TYPE_THUNDER:
            return L"Thunder";
        case ATTRIBUTE_TYPE::ATTRIBUTE_TYPE_DARK:
            return L"Water";
        default:
            return L"";
        }
    }

    AttributeState* GetState() const { return state_; }

    virtual bool StickTrigger(Vector2 stick, Vector2 previousStick) = 0;
    void AddPower();
    void AddPower(float scale);
    void AddPower(Vector2 stick);
    void FrictionPower();
    void Friction();
    Vector2 CalcVector(Vector2 stick);
};

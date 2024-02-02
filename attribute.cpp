#include "attribute.h"

void Attribute::AddPower(void) {
    power_ += state_->addPower;
if (state_->maxPower < power_) power_ = state_->maxPower;
else if (power_ < state_->minPower) power_ = state_->minPower;
}

void Attribute::AddPower(float scaler) {
    power_ += scaler * state_->inputPowerRate;
    if (state_->maxPower < power_) power_ = state_->maxPower;
    else if (power_ < state_->minPower) power_ = state_->minPower;
}

void Attribute::AddPower(Vector2 vector) {
    power_ += state_->addPower + vector.Distance() * state_->inputPowerRate;
    if (state_->maxPower < power_) power_ = state_->maxPower;
    else if (power_ < state_->minPower) power_ = state_->minPower;
}

void Attribute::FrictionPower(void) {
    power_ *= state_->powerFriction;
}

void Attribute::Friction(void) {
    player_->SetVel(player_->GetVel() * state_->friction);
}

Vector2 Attribute::CalcVector(Vector2 stick) {
    return stick * power_;
}

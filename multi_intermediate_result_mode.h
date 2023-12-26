#pragma once
#include "multi_mode.h"

class MultiPlayIntermediateResultModeServerSide : public MultiPlayServerSide {
public:
	MultiPlayIntermediateResultModeServerSide() : MultiPlayServerSide(nullptr) {
		timeLimit_ = 20.0f;
	}
};

class MultiPlayIntermediateResultModeClientSide : public MultiPlayClientSide {
public:
	MultiPlayIntermediateResultModeClientSide() : MultiPlayClientSide(nullptr) {};
};
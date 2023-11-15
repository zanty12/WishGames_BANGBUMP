#pragma once
#include <Xinput.h>
#include "lib/switch_check.h"

class Input {
private:
	static const int MAX_NUM = 8;
	SWITCH_CHECK<XINPUT_STATE> state[MAX_NUM];

public:
	enum KeyCode {
		Up,
		Down,
		Left,
		Right,

		East,
		West,
		North,
		South,

		L,
		R,
	};
	void Update(void);

	bool GetKey(KeyCode code);
};
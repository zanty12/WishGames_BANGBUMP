#include "win_xinput.h"

void Input::Update(void) {
	for (int i = 0; i < MAX_NUM; i++) {
		XINPUT_STATE state;
		XInputGetState(i, &state);
		this->state[i] = state;
	}
}

bool Input::GetKey(int index, KeyCode code) {
	if (index < 0 || MAX_NUM <= index) return false;
	XINPUT_STATE state = this->state[index].Get();

	switch (code)
	{
	case Input::Up:	state.Gamepad. break;
	case Input::Down:
		break;
	case Input::Left:
		break;
	case Input::Right:
		break;
	case Input::East:
		break;
	case Input::West:
		break;
	case Input::North:
		break;
	case Input::South:
		break;
	case Input::L:
		break;
	case Input::R:
		break;
	default:
		break;
	}
}

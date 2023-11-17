#include "xinput.h"

XINPUT_STATE Input::state[MAX_NUM] = {};
XINPUT_STATE Input::previous[MAX_NUM] = {};

Vector2 Input::to_vector2(SHORT stickX, SHORT stickY) {
    float deadZoneRatio = 0.2f;
    float x = (float)stickX / +32768.0f;
    float y = (float)stickY / -32768.0f;
    float distance = sqrtf(x * x + y * y);


    return deadZoneRatio < distance ? Vector2(x, y) : Vector2::Zero;
}





void Input::Update(void) {
    for (int i = 0; i < MAX_NUM; i++) {
        XINPUT_STATE state;
        XInputGetState(i, &state);
        Input::previous[i] = Input::state[i];
        Input::state[i] = state;
    }
}

bool Input::GetKey(XINPUT_GAMEPAD gamepad, KeyCode code) {
    WORD button = gamepad.wButtons;

    switch (code)
    {
    case Input::Up:    return button & XINPUT_GAMEPAD_DPAD_UP;
    case Input::Down: return button & XINPUT_GAMEPAD_DPAD_DOWN;
    case Input::Left: return button & XINPUT_GAMEPAD_DPAD_LEFT;
    case Input::Right: return button & XINPUT_GAMEPAD_DPAD_RIGHT;
    case Input::East: return button & XINPUT_GAMEPAD_X;
    case Input::West: return button & XINPUT_GAMEPAD_B;
    case Input::North: return button & XINPUT_GAMEPAD_A;
    case Input::South: return button & XINPUT_GAMEPAD_Y;
    case Input::L: return button & XINPUT_GAMEPAD_LEFT_SHOULDER;
    case Input::R: return button & XINPUT_GAMEPAD_RIGHT_SHOULDER;
    default: return false;
    }
}

Vector2 Input::GetStickRight(int index) {
    SHORT x = state[index].Gamepad.sThumbRX;
    SHORT y = state[index].Gamepad.sThumbRY;

    return to_vector2(x, y);
}

Vector2 Input::GetStickLeft(int index) {
    SHORT x = state[index].Gamepad.sThumbLX;
    SHORT y = state[index].Gamepad.sThumbLY;

    return to_vector2(x, y);
}

bool Input::GetKey(int index, KeyCode code) {
    if (index < 0 || MAX_NUM <= index) return false;
    return GetKey(state[index].Gamepad, code);
}
bool Input::GetKeyUp(int index, KeyCode code) {
    if (index < 0 || MAX_NUM <= index) return false;
    return !GetKey(state[index].Gamepad, code) && GetKey(previous[index].Gamepad, code);
}
bool Input::GetKeyDown(int index, KeyCode code) {
    if (index < 0 || MAX_NUM <= index) return false;
    return GetKey(state[index].Gamepad, code) && !GetKey(previous[index].Gamepad, code);
}
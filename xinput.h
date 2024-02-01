#pragma once
#include "lib\vector.h"
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "xinput.lib")

class Input {
private:
	static const int MAX_NUM = 8;
	static XINPUT_STATE state[MAX_NUM];
	static XINPUT_STATE previous[MAX_NUM];

public:
	enum KeyCode {
		Up,
		Down,
		Left,
		Right,

		West,
		East,
		North,
		South,

		X = West,
		B = East,
		Y = North,
		A = South,

		L,
		R,
		LThumb,
		RThumb,
	};



private:
	static Vector2 to_vector2(SHORT stickX, SHORT stickY);

public:
	static void Update(void);
	static bool GetKey(int index, KeyCode code);
	static bool GetKeyUp(int index, KeyCode code);
	static bool GetKeyDown(int index, KeyCode code);
	static bool GetKey(XINPUT_GAMEPAD gamepad, KeyCode code);
	static Vector2 GetStickRight(int index);
	static Vector2 GetStickLeft(int index);

	static float GetTriggerRight(int index);
	static float GetTriggerLeft(int index);

	static void Vibration(int index, float left, float right);

	static XINPUT_GAMEPAD GetState(int index) { return state[index].Gamepad; }
	static XINPUT_GAMEPAD GetPreviousState(int index) { return previous[index].Gamepad; }
	static void SetState(int index, XINPUT_GAMEPAD gamepad) { state[index].Gamepad = gamepad; }
	static void SetPreviousState(int index, XINPUT_GAMEPAD gamepad) { previous[index].Gamepad = gamepad; }

	static Vector2 GetPreviousStickRight(int index);
	static Vector2 GetPreviousStickLeft(int index);

};
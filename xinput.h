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

	static Vector2 GetPreviousStickRight(int index);
	static Vector2 GetPreviousStickLeft(int index);

};
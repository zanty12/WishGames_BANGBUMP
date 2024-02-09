#include "win_input.h"

bool WIN::Input::keys[256] = { };
bool WIN::Input::olds[256] = { };

namespace WIN {
	bool Input::GetKey(unsigned int key) {
		return (keys[key] & 0x80);
	}

	bool Input::GetKeyUp(unsigned int key) {
		return !(keys[key] & 0x80) && (olds[key] & 0x80);
	}

	bool Input::GetKeyDown(unsigned int key) {
		return (keys[key] & 0x80) && !(olds[key] & 0x80);
	}

	Vector2Int Input::GetMousePosition(void) {
		POINT point;
		GetCursorPos(&point);
		point.y *= -1;
		return Vector2Int(point.x, point.y);
	}

	Vector2Int Input::GetMousePosition(HWND hwnd) {
		RECT rect = {};
		POINT point;
		GetWindowRect(hwnd, &rect);
		GetCursorPos(&point);
		Vector2Int position = Vector2Int(point.x, point.y) - Vector2Int(rect.left, rect.top);
		position.y *= -1;		
		return position;
	}

	void Input::Update(void) {
		memcpy(olds, keys, sizeof(bool) * 256);
		GetKeyboardState((BYTE *)&keys);
	}
}

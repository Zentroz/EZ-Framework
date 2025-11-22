#include"Engine/Input.h"

void Input::KeyDown(unsigned char keycode) noexcept {
	if (keycode == 'L') LockCursor(!lockCursor);

	m_keyStates.set(keycode);
}
void Input::KeyUp(unsigned char keycode) noexcept {
	m_keyStates.reset(keycode);
}

bool Input::GetKey(KeyCode keycode) const noexcept {
	return m_keyStates.test(keycode);
}

void Input::UpdateMousePosition(HWND hWnd) {
	POINT p;
	if (GetCursorPos(&p)) {
		ScreenToClient(hWnd, &p);
		float2 pos = float2(p.x, p.y);

		delta = pos - mousePosition;

		mousePosition = pos;
	}

	if (lockCursor) {
		p.x = 300;
		p.y = 300;
		ClientToScreen(hWnd, &p);

		SetCursorPos(p.x, p.y);

		ScreenToClient(hWnd, &p);
		mousePosition = float2(p.x, p.y);
	}
}

void Input::LockCursor(bool lock) {
	ShowCursor(!lock);
	lockCursor = lock;
}
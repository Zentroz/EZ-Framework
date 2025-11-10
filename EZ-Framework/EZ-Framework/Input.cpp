#include"Input.h"

void Input::KeyDown(unsigned char keycode) noexcept {
	m_keyStates.set(keycode);
}
void Input::KeyUp(unsigned char keycode) noexcept {
	m_keyStates.reset(keycode);
}

bool Input::GetKey(unsigned char keycode) const noexcept {
	return m_keyStates.test(keycode);
}
#ifndef INPUT_CLASS_H
#define INPUT_CLASS_H

#include<bitset>

class Input {
public:
	void KeyDown(unsigned char keycode) noexcept;
	void KeyUp(unsigned char keycode) noexcept;

	bool GetKey(unsigned char keycode) const noexcept;
private:
	std::bitset<256> m_keyStates;
};

#endif
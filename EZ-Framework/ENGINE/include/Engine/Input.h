#ifndef INPUT_CLASS_H
#define INPUT_CLASS_H

#include"Core/ChiliWin.h"

#include<bitset>

class Input {
public:
	enum KeyCode : unsigned char {
		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D',
		E = 'E',
		F = 'F',
		G = 'G',
		H = 'H',
		I = 'I',
		J = 'J',
		K = 'K',
		L = 'L',
		M = 'M',
		N = 'N',
		O = 'O',
		P = 'P',
		Q = 'Q',
		R = 'R',
		S = 'S',
		T = 'T',
		U = 'U',
		V = 'V',
		W = 'W',
		X = 'X',
		Y = 'Y',
		Z = 'Z',
		SPACE = VK_SPACE,
		LSHIFT = VK_LSHIFT,
		RSHIFT = VK_RSHIFT,
		LCTRL = VK_LCONTROL,
		RCTRL = VK_RCONTROL,
		ENTER = VK_RETURN,
		ESCAPE = VK_ESCAPE
	};
public:
	void KeyDown(unsigned char keycode) noexcept;
	void KeyUp(unsigned char keycode) noexcept;

	bool GetKey(KeyCode keycode) const noexcept;
private:
	std::bitset<256> m_keyStates;
};

#endif
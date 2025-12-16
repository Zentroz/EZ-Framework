#ifndef INPUT_CLASS_H
#define INPUT_CLASS_H

#include<bitset>

#include"Engine/Core/Math.h"
#include"Engine/Core/ChiliWin.h"
#include"Engine/Core/KeyCodes.h"

using namespace MATH;

namespace ENGINE {

	class Input {
	public:
		void KeyDown(unsigned char keycode) noexcept;
		void KeyUp(unsigned char keycode) noexcept;
		void UpdateMousePosition(HWND hWnd);
		void LockCursor(bool lock);

		bool GetKey(KeyCode keycode) const noexcept;
		float2 GetMousePosition() { return mousePosition; }
		float2 GetMouseDelta() { return delta; }

	private:
		std::bitset<256> m_keyStates;
		float2 mousePosition;
		float2 delta;

		bool lockCursor = false;
	};

}

#endif
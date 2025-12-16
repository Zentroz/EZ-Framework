#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include"Engine/Events/Event.h"
#include"Engine/Core/KeyCodes.h"

enum KeyState { Pressed, Hold, Released };

class KeyEvent : public Event {
public:
	KeyCode key;
	KeyState state;
	KeyEvent(KeyCode key, KeyState state) : key(key), state(state) {}

	EVENT_TYPE_FUNCS(Keyboard)
};

class MouseButtonEvent : public Event {
public:
	int button;
	KeyState state;
	MouseButtonEvent(int button, KeyState state) : button(button), state(state) {}

	EVENT_TYPE_FUNCS(MouseButton)
};

#endif
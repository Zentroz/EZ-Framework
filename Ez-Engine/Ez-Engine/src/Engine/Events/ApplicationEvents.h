#pragma once

#include"Engine/Events/Event.h"

class ApplicationQuit : public Event {
public:
	ApplicationQuit() = default;

	EVENT_TYPE_FUNCS(ApplicationClosed);
};
#ifndef EVENT_CLASS_H
#define EVENT_CLASS_H

enum EventType {
	None, ApplicationClosed, WindowResized, Keyboard, MouseButton
};

#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define EVENT_TYPE_FUNCS(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }

class Event {
public:

	virtual EventType GetEventType() const = 0;
};


class EventDispatcher {
public:
	EventDispatcher(Event& e) : m_Event(e) {}

	template <typename T, typename F>
	bool Dispatch(const F& func) {
		if (m_Event.GetEventType() == T::GetStaticType()) {
			func(static_cast<T&>(m_Event));
			return true;
		}

		return false;
	}

private:
	Event& m_Event;
};

#endif
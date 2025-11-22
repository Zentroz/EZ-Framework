#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include<functional>
#include<unordered_map>
#include<typeindex>

class Registry;

template<typename T>
struct ComponentAddedEvent {
	uint32_t entity;
	T& component;
	Registry* registry;

	ComponentAddedEvent(uint32_t entity, T& component, Registry* registry) : entity(entity), component(component), registry(registry) {}
};

class EventBus {
public:
	template<typename T>
	static void Subscribe(std::function<void(const T&)> callback);

	template<typename T>
	static void Emit(const T& event);

private:
	static std::unordered_map<std::type_index, std::vector<std::function<void(void*)>>> subscribers;
};

template<typename T>
void EventBus::Subscribe(std::function<void(const T&)> callback) {
	auto& vec = subscribers[typeid(T)];
	vec.push_back(
		[cb = std::move(callback)](void* data) {
			cb(*static_cast<T*>(data));
		}
	);
}

template<typename T>
void EventBus::Emit(const T& event) {
	auto it = subscribers.find(typeid(T));
	if (it == subscribers.end()) return;

	for (auto& cb : it->second)
		cb((void*)&event);
}

#endif
#ifndef LAYER_CLASS_H
#define LAYER_CLASS_H

#include"Engine/Events/Event.h"

class Layer {
public:
	virtual ~Layer() = default;

	// Init and End
	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnEvent(Event& event) {}
	// Update
	virtual void OnUpdate() {}
	// Rendering
	virtual void OnRender() {}
	virtual void OnRenderImGui() {}

private:
};

#endif
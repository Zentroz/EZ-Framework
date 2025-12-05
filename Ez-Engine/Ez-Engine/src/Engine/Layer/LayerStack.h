#ifndef LAYER_STACK_CLASS_H
#define LAYER_STACK_CLASS_H

#include<vector>

#include"Engine/Layer/Layer.h"

class LayerStack {
public:
	virtual ~LayerStack() = default;

	void AttachLayer(Layer* layer) {
		m_layers.push_back(layer);
		layer->OnAttach();
	}
	void AttachOverlay(Layer* layer) {
		m_overlays.push_back(layer);
		layer->OnAttach();
	}

	template<typename T>
	void DetachLayer() {}
	template<typename T>
	void DetachOverlay() {}

	std::vector<Layer*> Layers() { return m_layers; }
	std::vector<Layer*> Overlays() { return m_overlays; }

private:
	std::vector<Layer*> m_layers;
	std::vector<Layer*> m_overlays;
};

#endif
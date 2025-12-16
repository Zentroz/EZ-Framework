#pragma once

#include"Engine/Engine.h"
#include"Engine/Events/Event.h"

class Application {
public:
	Application();

	void Shutdown();
	void Init();
	void Run();
	void CloseApplication();

	void OnEvent(Event& event);

	// Layer Stack
	void PushLayer(Layer* layer) { m_Engine.Get().LayerStack().AttachLayer(layer); }
	void PushOverlay(Layer* layer) { m_Engine.Get().LayerStack().AttachOverlay(layer); }

	void PopLayer() { m_Engine.Get().LayerStack().DetachLayer<Layer>(); }
	void PopOverlay() { m_Engine.Get().LayerStack().DetachOverlay<Layer>(); }

	static Application& Instance() { return *m_Application; }

private:
	static Application* m_Application;
	ENGINE::Engine m_Engine;
	bool isRunning;
};
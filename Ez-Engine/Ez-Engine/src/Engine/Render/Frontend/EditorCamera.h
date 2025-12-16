#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include"Engine/Events/Event.h"
#include"Engine/Events/InputEvents.h"
#include"Engine/Render/Frontend/Camera.h"

namespace ENGINE {
	class EditorCamera {
	public:
		EditorCamera();

		void OnEvent(Event& event);
		void OnUpdate();
		void OnKeyEvent(KeyEvent& e);
		void OnMouseButtonEvent(MouseButtonEvent& e);

		RENDERER::Camera& GetCamera() { return m_Camera; };
	private:
		RENDERER::Camera m_Camera;
		float3 moveInput;
		float m_Speed;
		bool fpsMode;
	};
}

#endif
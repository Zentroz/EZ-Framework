#include"Engine/Render/Frontend/EditorCamera.h"
#include"Engine/Core/GameTime.h"

namespace ENGINE {

	EditorCamera::EditorCamera() : fpsMode(false), m_Speed(5) {
		m_Camera.position = float3(0, 0, -10);
		m_Camera.rotation = quaternion::Identity();
	}

	void EditorCamera::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<KeyEvent>(BIND_EVENT_FN(EditorCamera::OnKeyEvent));
		dispatcher.Dispatch<MouseButtonEvent>(BIND_EVENT_FN(EditorCamera::OnMouseButtonEvent));
	}

	void EditorCamera::OnUpdate() {
		float3 moveDir = m_Camera.rotation.forward() * moveInput.z + m_Camera.rotation.right() * moveInput.x + float3::up() * moveInput.y;
		float3 velocity = moveDir * GameTime::deltaTime * m_Speed;
		m_Camera.position += velocity;
	}

	void EditorCamera::OnKeyEvent(KeyEvent& e) {
		if (e.key == KeyCode::W && (e.state == KeyState::Hold || e.state == KeyState::Pressed)) {
			moveInput.z = 1;
		}
		else if (e.key == KeyCode::S && (e.state == KeyState::Hold || e.state == KeyState::Pressed)) {
			moveInput.z = -1;
		}
		else if ((e.key == KeyCode::W && e.state == KeyState::Released) || (e.key == KeyCode::S && e.state == KeyState::Released)) {
			moveInput.z = 0;
		}

		if (e.key == KeyCode::D && (e.state == KeyState::Hold || e.state == KeyState::Pressed)) {
			moveInput.x = 1;
		}
		else if (e.key == KeyCode::A && (e.state == KeyState::Hold || e.state == KeyState::Pressed)) {
			moveInput.x = -1;
		}
		else if ((e.key == KeyCode::D && e.state == KeyState::Released) || (e.key == KeyCode::A && e.state == KeyState::Released)) {
			moveInput.x = 0;
		}

		if (e.key == KeyCode::SPACE && (e.state == KeyState::Hold || e.state == KeyState::Pressed)) {
			moveInput.y = 1;
		}
		else if (e.key == KeyCode::LCTRL && (e.state == KeyState::Hold || e.state == KeyState::Pressed)) {
			moveInput.y = -1;
		}
		else if ((e.key == KeyCode::SPACE && e.state == KeyState::Released) || (e.key == KeyCode::LCTRL && e.state == KeyState::Released)) {
			moveInput.y = 0;
		}
	}

	void EditorCamera::OnMouseButtonEvent(MouseButtonEvent& e) {

	}

}
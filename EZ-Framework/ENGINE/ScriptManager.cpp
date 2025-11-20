#include"Engine/ScriptManager.h"

ScriptManager::ScriptManager() : scriptsCount(0) {
	m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

	try {
		m_lua.safe_script_file("Assets/Scripts/table.lua");
	}
	catch (const sol::error& e) {
		EXCEPTION(e.what());
	}

	std::string type = m_lua["type"]();
	int i = 0;
}

void ScriptManager::Init(Input* input, Camera* camera, Registry* registry) {
	RegisterWrapperFunctions(input, camera, registry);
}

bool ScriptManager::LoadScript(std::string path) {
	if (loadedScripts.contains(path)) return false;

	 sol::environment env = sol::environment(m_lua, sol::create, m_lua.globals());

	try {
		m_lua.safe_script_file(path, env);
	}
	catch (const sol::error& e) {
		EXCEPTION(e.what());
		return false;
	}

	loadedScripts[path] = env;

	return true;
}

void ScriptManager::Update() {
	for (auto& pair : loadedScripts) {
		sol::function updateFunc = pair.second["update"];

		if (updateFunc.valid()) {
			try {
				updateFunc();
			}
			catch (const sol::error& e) {
				EXCEPTION(e.what());
			}
		}
	}
}

void ScriptManager::RegisterWrapperFunctions(Input* input, Camera* camera, Registry* registry) {
	// Math
	RegisterMathWrappers();

	// Transform
	RegisterComponentWrappers();
	
	// Physics
	// Material
	RegisterMaterialWrappers();

	// Camera
	RegisterCameraWrappers(camera);

	// Input
	RegisterInputWrappers(input);

	RegisterECSWrappers(registry);

	// Utility
	RegisterTimeWrappers();

	m_lua["OutputDebug"] = [](std::string msg) {
		OutputDebugStringA((msg + "\n").c_str());
	};
}

void ScriptManager::RegisterECSWrappers(Registry* registry) {
	sol::table game = m_lua.create_named_table("Game");

	game.set_function("CreateEntity",
		[registry]() {
			return registry->CreateEntity();
		}
	);

	game.set_function("DestroyEntity",
		[registry](int id) {
			return registry->DestroyEntity(id);
		}
	);

	game.set_function("AddComponent",
		[registry](int id, sol::object const& obj) {
			if (obj.is<TransformComponent>()) {
				TransformComponent t = obj.as<TransformComponent>();
				registry->AddComponent(id, &t);
			}
			else {
				EXCEPTION("Component type not recognized");
			}
		}
	);

	game.set_function("GetEntitiesWithComponents",
		[registry](sol::table tab) {
			std::vector<int> components{};

			for (size_t i = 1; i <= tab.size(); i++)
			{
				components.push_back(tab[i].get_or(-1));
			}

			return registry->CreateEntity();
		}
	);

	enum LuaComponent
	{
		Transform
	};

	m_lua.new_enum("Component",
		"Transform", LuaComponent::Transform
	);

	sol::state* lua = &m_lua;

	game.set_function("GetComponent",
		[lua, registry](int id, LuaComponent component) {
			sol::state_view view(*lua);
			IComponent* comp = nullptr;
			if (component == Transform) {
				comp = registry->GetComponent<TransformComponent>(id);
			}
			else {
				EXCEPTION(("There is no Component: " + std::to_string(component)).c_str());
			}

			if (comp == nullptr) {
				EXCEPTION(("Entity does not have Component: " + std::to_string(component)).c_str());
			}

			return sol::make_object(view.lua_state(), comp);
		}
	);
}

void ScriptManager::RegisterComponentWrappers() {
	m_lua.new_usertype<IComponent>("Component");

	TransformComponent::RegisterLuaWrapper(m_lua);
}

void ScriptManager::RegisterCameraWrappers(Camera* camera) {
	sol::table cam = m_lua.create_named_table("Camera");

	float3* camPosition = &camera->position;

	cam.set("position", camPosition);
	cam["rotation"] = &camera->rotation;

	cam.set_function("GetPosition",
		[camera]() {
			return camera->position;
		}
	);

	cam.set_function("SetPosition",
		[camera](float3 position) {
			camera->SetPosition(position);
		}
	);

	cam.set_function("GetRotation",
		[camera]() {
			return camera->rotation;
		}
	);

	cam.set_function("SetRotation",
		[camera](quaternion rotation) {
			camera->rotation = rotation;
		}
	);
}

void ScriptManager::RegisterMaterialWrappers() {
	sol::table material = m_lua.create_named_table("Material");

	material.set_function("SetFloat", 
		[](uint8_t entity, float value) {

		}
	);
}

void ScriptManager::RegisterInputWrappers(Input* input) {
	m_lua.new_enum("Key",
		"A", Input::KeyCode::A,
		"B", Input::KeyCode::B,
		"C", Input::KeyCode::C,
		"D", Input::KeyCode::D,
		"E", Input::KeyCode::E,
		"F", Input::KeyCode::F,
		"G", Input::KeyCode::G,
		"H", Input::KeyCode::H,
		"I", Input::KeyCode::I,
		"J", Input::KeyCode::J,
		"K", Input::KeyCode::K,
		"L", Input::KeyCode::L,
		"M", Input::KeyCode::M,
		"N", Input::KeyCode::N,
		"O", Input::KeyCode::O,
		"P", Input::KeyCode::P,
		"Q", Input::KeyCode::Q,
		"R", Input::KeyCode::R,
		"S", Input::KeyCode::S,
		"T", Input::KeyCode::T,
		"U", Input::KeyCode::U,
		"V", Input::KeyCode::V,
		"W", Input::KeyCode::W,
		"X", Input::KeyCode::X,
		"Y", Input::KeyCode::Y,
		"Z", Input::KeyCode::Z,
		"SPACE", Input::KeyCode::SPACE,
		"LSHIFT", Input::KeyCode::LSHIFT,
		"RSHIFT", Input::KeyCode::RSHIFT,
		"LCTRL", Input::KeyCode::LCTRL,
		"RCTRL", Input::KeyCode::RCTRL,
		"ENTER", Input::KeyCode::ENTER,
		"ESCAPE", Input::KeyCode::ESCAPE
	);

	sol::table inputTable = m_lua.create_named_table("Input");

	inputTable.set_function("GetMousePosition",
		[input]() {
			return input->GetMousePosition();
		}
	);

	inputTable.set_function("GetMouseDelta",
		[input]() {
			return input->GetMouseDelta();
		}
	);

	inputTable.set_function("GetKey",
		[input](Input::KeyCode key) {
			bool isKey = input->GetKey(key);

			return isKey;
		}
	);
}

void ScriptManager::RegisterMathWrappers() {
	m_lua.new_usertype<float3>("Float3",
		sol::constructors<float3(), float3(float, float, float)>(),
		"x", &float3::x,
		"y", &float3::y,
		"z", &float3::z,
		"One", &float3::one,
		"Zero", &float3::zero,
		"Up", &float3::up,
		"Forward", &float3::forward,
		"Right", &float3::right,
		"Length", &float3::length,
		"Normalize", &float3::normalize,
		//"dot", &float3::dot,
		//"cross", &float3::cross

		sol::meta_function::addition, sol::resolve<float3(const float3&) const>(&float3::operator+),
		sol::meta_function::subtraction, sol::resolve<float3(const float3&) const>(&float3::operator-),
		sol::meta_function::multiplication, sol::overload(sol::resolve<float3(float) const>(&float3::operator*))
	);

	m_lua.new_usertype<float2>("Float2",
		sol::constructors<float2(), float2(float, float)>(),
		"x", &float2::x,
		"y", &float2::y,
		"One", &float2::one,
		"Zero", &float2::zero,
		"Up", &float2::up,
		"Right", &float2::right,
		"Length", &float2::length,
		"Normalize", &float2::normalize,
		//"dot", &float3::dot,
		//"cross", &float3::cross

		sol::meta_function::addition, sol::resolve<float2(const float2&) const>(&float2::operator+),
		sol::meta_function::subtraction, sol::resolve<float2(const float2&) const>(&float2::operator-),
		sol::meta_function::multiplication, sol::overload(sol::resolve<float2(float) const>(&float2::operator*))
	);

	m_lua.new_usertype<quaternion>("Quaternion",
		sol::constructors<quaternion(), quaternion(float, float, float, float)>(),
		"x", &quaternion::x,
		"y", &quaternion::y,
		"z", &quaternion::z,
		"w", &quaternion::w,
		"Identity", &quaternion::Identity,
		"Forward", &quaternion::forward,
		"Up", &quaternion::up,
		"Right", &quaternion::right,
		"Normalize", &quaternion::normalize,
		"FromAxisAngle", sol::factories([](float3 axis, float angle) {
			return quaternion::FromAxisAngle(axis, angle);
		}),
		sol::meta_function::multiplication, sol::overload(sol::resolve<quaternion(const quaternion&) const>(&quaternion::operator*))
	);
}

void ScriptManager::RegisterTimeWrappers() {
	sol::table time = m_lua.create_named_table("Time");

	time.set_function("GetTotalTime",
		[]() {
			return GameTime::time;
		}
	);

	time.set_function("GetDeltaTime",
		[]() {
			return GameTime::deltaTime;
		}
	);
}
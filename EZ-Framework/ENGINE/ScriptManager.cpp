#include"Engine/ScriptManager.h"

ScriptManager::ScriptManager() {
	m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
}

void ScriptManager::Init(Input& input, Registry* registry, Camera* camera) {
	RegisterWrapperFunctions(input, registry, camera);

	LoadScripts(registry);
}

bool ScriptManager::LoadScripts(Registry* registry) {

	std::vector<ScriptComponent*> scripts = registry->GetAllComponentsOfType<ScriptComponent>();

	for (ScriptComponent* script : scripts) {
		script->luaEnv = sol::environment(m_lua, sol::create, m_lua.globals());

		auto my = script->luaEnv.create_named("MY");
		my["id"] = script->entity;

		try {
			m_lua.safe_script_file(script->path, script->luaEnv);
		}
		catch (const sol::error& e) {
			EXCEPTION(e.what());
			return false;
		}
	}

	return true;
}

void ScriptManager::Start(Registry* registry) {
	std::vector<ScriptComponent*> scripts = registry->GetAllComponentsOfType<ScriptComponent>();

	for (ScriptComponent* script : scripts) {
		sol::function updateFunc = script->luaEnv["start"];
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

void ScriptManager::Update(Registry* registry) {
	std::vector<ScriptComponent*> scripts = registry->GetAllComponentsOfType<ScriptComponent>();

	for (ScriptComponent* script : scripts) {
		sol::function updateFunc = script->luaEnv["update"];
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

void ScriptManager::RegisterWrapperFunctions(Input& input, Registry* registry, Camera* camera) {
	// ECS
	RegisterECSWrappers(registry);

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

	game.set_function("GetComponent",
		[registry](int id, LuaComponent component) {
			if (component == Transform) {
				TransformComponent* t = registry->GetComponent<TransformComponent>(id);

				if (t == nullptr) {
					EXCEPTION("Entity does not have Transform component");
				}

				return t;
			}
		}
	);
}

void ScriptManager::RegisterComponentWrappers() {
	TransformComponent::RegisterWrapper(&m_lua);
}

void ScriptManager::RegisterCameraWrappers(Camera* camera) {
	sol::table cam = m_lua.create_named_table("Camera");

	cam.set_function("SetPosition",
		[camera](float3 position) {
			camera->SetPosition(position);
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

void ScriptManager::RegisterInputWrappers(Input& input) {
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

	inputTable.set_function("GetKey",
		[&](Input::KeyCode key) {
			return input.GetKey(key);
		}
	);
}

void ScriptManager::RegisterMathWrappers() {
	m_lua.new_usertype<float3>("Float3",
		sol::constructors<float3(), float3(float, float, float)>(),
		"x", &float3::x,
		"y", &float3::y,
		"z", &float3::z,
		"length", &float3::length,
		"normalize", &float3::normalize,
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
		"length", &float2::length,
		"normalize", &float2::normalize,
		//"dot", &float3::dot,
		//"cross", &float3::cross

		sol::meta_function::addition, sol::resolve<float2(const float2&) const>(&float2::operator+),
		sol::meta_function::subtraction, sol::resolve<float2(const float2&) const>(&float2::operator-),
		sol::meta_function::multiplication, sol::overload(sol::resolve<float2(float) const>(&float2::operator*))
	);
}

void ScriptManager::RegisterTimeWrappers() {
	sol::table time = m_lua.create_named_table("Time");

	time.set_function("GetDeltaTime",
		[]() {
			return Time::deltaTime;
		}
	);
}
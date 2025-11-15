#include"ScriptManager.h"

ScriptManager::ScriptManager() {
	m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
}

bool ScriptManager::LoadScript(const char* path) {
	try {
		m_lua.safe_script_file(path);
	}
	catch (const sol::error& e) {
		EXCEPTION(e.what());
		return false;
	}

	return true;
}

void ScriptManager::Update() {
	m_lua["update"](Time::deltaTime);
}

void ScriptManager::RegisterWrapperFunctions(Input& input, Registry* registry) {
	// ECS
	RegisterECSWrappers(registry);

	// Transform
	RegisterTransformWrappers();
	
	// Math
	RegisterMathWrappers();
	
	// Physics
	// Material
	RegisterMaterialWrappers();

	// Input
	RegisterInputWrappers(input);

	// Utility
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
}

void ScriptManager::RegisterTransformWrappers() {

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
		"SPACE", Input::KeyCode::SPACE,
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
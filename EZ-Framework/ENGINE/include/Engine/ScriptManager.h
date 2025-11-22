#ifndef SCRIPT_MANAGER_CLASS_H
#define SCRIPT_MANAGER_CLASS_H

#define SOL_ALL_SAFETIES_ON 1
#include"sol/sol.hpp";
#pragma comment(lib, "lua54.lib")

#include"Core/Math.h"
#include"Core/GameTime.h"
#include"Core/ErrorHandler.h"

#include"Input.h"
#include"Registry.h"
#include"Camera.h"

#include"ECSComponents.h"


class ScriptManager {
public:
	ScriptManager(Registry* registry);

	void Init(Input* input, Camera* camera, Registry* registry);

	void RegisterWrapperFunctions(Input* input, Camera* camera, Registry* registry);

#pragma region Wrappers

	void RegisterInputWrappers(Input* input);
	void RegisterMathWrappers();
	void RegisterCameraWrappers(Camera* camera);
	void RegisterMaterialWrappers();
	void RegisterEngineComponent(Registry* registry);
	void RegisterTimeWrappers();
	void RegisterECSWrappers(Registry* registry);

#pragma endregion

	bool LoadScript(std::string path);
	void Update();
private:
	sol::state m_lua;
	bool calledAwake = false;
	std::unordered_map<std::string, sol::environment> loadedScripts;
	uint8_t scriptsCount;

	Registry* registry;
};

#endif
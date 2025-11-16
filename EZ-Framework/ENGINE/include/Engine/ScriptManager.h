#ifndef SCRIPT_MANAGER_CLASS_H
#define SCRIPT_MANAGER_CLASS_H

#define SOL_ALL_SAFETIES_ON 1
#include"sol/sol.hpp";
#pragma comment(lib, "lua54.lib")

#include"Core/Math.h"
#include"Core/Time.h"
#include"Core/ErrorHandler.h"

#include"Input.h"
#include"Registry.h"
#include"Camera.h"
#include"ECSComponents.h"


class ScriptManager {
public:
	ScriptManager();

	void Init(Input& input, Registry* registry, Camera* camera);

	void RegisterWrapperFunctions(Input& input, Registry* registry, Camera* camera);

#pragma region Wrappers

	void RegisterInputWrappers(Input& input);
	void RegisterMathWrappers();
	void RegisterCameraWrappers(Camera* camera);
	void RegisterMaterialWrappers();
	void RegisterComponentWrappers();
	void RegisterTimeWrappers();
	void RegisterECSWrappers(Registry* registry);

#pragma endregion

	bool LoadScripts(Registry* registry);
	void Start(Registry* registry);
	void Update(Registry* registry);
private:
	sol::state m_lua;
};

#endif
#ifndef SCRIPT_MANAGER_CLASS_H
#define SCRIPT_MANAGER_CLASS_H

#include"ErrorHandler.h"
#include"Time.h"
#include"Math.h"
#include"Input.h"
#include"Registry.h"

#define SOL_ALL_SAFETIES_ON 1
#include"sol/sol.hpp";
#pragma comment(lib, "lua54.lib")


class ScriptManager {
public:
	ScriptManager();

	void RegisterWrapperFunctions(Input& input, Registry* registry);

#pragma region Wrappers

	void RegisterInputWrappers(Input& input);
	void RegisterMathWrappers();
	void RegisterMaterialWrappers();
	void RegisterTransformWrappers();
	void RegisterECSWrappers(Registry* registry);

#pragma endregion
	bool LoadScript(const char* path);
	void Update();
private:
	sol::state m_lua;
};

#endif
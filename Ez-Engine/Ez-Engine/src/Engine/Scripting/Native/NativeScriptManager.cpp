#include"Engine/Scripting/Native/NativeScriptManager.h"
#include"Engine/Core/ChiliWin.h"
#include<string>

using RegisterGameFn = void(*)();
using CreateScriptFn = EZ::Script*(*)();

namespace ENGINE {
	void NativeScriptManger::Init() {
		HMODULE gameDLL = LoadLibrary(L"Game.dll");
		
		if (gameDLL) {
			auto registerFn = (RegisterGameFn)GetProcAddress(gameDLL, "RegisterGame");
			registerFn();

			auto testFn = (CreateScriptFn)GetProcAddress(gameDLL, "Test");
			EZ::Script* script = testFn();

			script->gameObject;

			m_Scripts["Test"] = script;
		}
	}

	void NativeScriptManger::Shutdown() {

	}

	void NativeScriptManger::Update() {
		for (auto [key, value] : m_Scripts) {
			value->OnUpdate(0.001f);
		}
	}
}
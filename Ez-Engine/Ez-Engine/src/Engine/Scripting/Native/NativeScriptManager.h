#pragma once

#include<unordered_map>

#include<EzAPI/Core/Script.h>

namespace ENGINE {
	class NativeScriptManger {
	public:
		NativeScriptManger() = default;

		void Init();
		void Shutdown();

		void Update();
	private:
		std::unordered_map<std::string, EZ::Script*> m_Scripts;
	};
}
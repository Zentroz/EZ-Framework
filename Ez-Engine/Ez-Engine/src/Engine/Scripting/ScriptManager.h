#ifndef SCRIPT_MANAGER_CLASS_H
#define SCRIPT_MANAGER_CLASS_H

#include<vector>
#include<fstream>
#include<functional>
#include<unordered_map>

#include<mono/jit/jit.h>
#include<mono/metadata/assembly.h>
#include<mono/metadata/attrdefs.h>
#include<mono/utils/mono-error.h>
#include<mono/utils/mono-logger.h>

#include"Engine/Core/ChiliWin.h"
#include"Engine/Core/EventBus.h"
#include"Engine/Core/ErrorHandler.h"
#include"Engine/Core/SparceSet.h"
#include"Engine/ECS/Registry.h"
#include"Engine/Render/Backend/Input.h"

namespace ENGINE {
	namespace SCRIPT {
		class ScriptRuntime {
		public:
			ScriptRuntime() = default;
			~ScriptRuntime() { ShutdownMono(); }
			void Init();
			void InitMono();
			void ShutdownMono();
			
			void RegisterInternalCalls();
			void LoadGameComponentScripts();
			void CreateBehviourInstance(Entity entity, std::string key);

			void OnEntityCreatedCallback(Entity id);
			void OnEntityDestroyedCallback(Entity id);

			void Update();

			static void SetRegistry(ECS::Registry* registry) { ScriptRuntime::registry = registry; }
			static void SetInput(Input* input) { ScriptRuntime::input = input; }
			static MonoDomain* GetAppDomain() {  return s_AppDomain; }
			static ECS::Registry* GetRegistry() { return registry; }
			static Input* GetInput() { return input; }

		private:
			static MonoDomain* s_AppDomain;
			MonoDomain* s_RootDomain = nullptr;
			MonoAssembly* s_EngineAssembly = nullptr;
			MonoAssembly* s_GameAssembly = nullptr;

			// Base Behaviour Class
			MonoClass* entityKlass = nullptr;
			MonoClass* entityBehaviour = nullptr;

			// Loaded Behaviour scripts
			std::vector<std::unique_ptr<ScriptObject>> m_scripts;
			std::unordered_map<std::string, MonoClass*> behaviourKlasses;
			std::unordered_map<Entity, MonoObject*> entityInstancesCS;
			static ECS::Registry* registry;
			static Input* input;
		};
	}
}

#endif
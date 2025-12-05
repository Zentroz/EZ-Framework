#include"Engine/Core/GameTime.h"
#include"Engine/Scripting/ScriptManager.h"

namespace ENGINE {
	namespace SCRIPT {

#pragma region Entity

		extern "C" MonoString* GetEntityName(uint32_t id) {
			ECS::GameEntity* entity = ScriptRuntime::GetRegistry()->GetGameEntity(id);
			if (entity == nullptr) return mono_string_new(ScriptRuntime::GetAppDomain(), "");;
			return mono_string_new(ScriptRuntime::GetAppDomain(), entity->name.c_str());
		}

		extern "C" bool HasComponent(uint32_t id, MonoReflectionType* reflectionType) {
			MonoType* type = mono_reflection_type_get_type(reflectionType);
			const char* typeName = mono_type_get_name(type);

			return false;
		}
		
		extern "C" void AddComponent(uint32_t id, MonoReflectionType* reflectionType) {
			MonoType* type = mono_reflection_type_get_type(reflectionType);
			MonoClass* klass = mono_class_from_mono_type(type);

			MonoObject* instance = mono_object_new(ScriptRuntime::GetAppDomain(), klass);

			mono_runtime_object_init(instance);

			MonoClassField* field = mono_class_get_field_from_name(klass, "EntityID");
			if (field)
				mono_field_set_value(instance, field, &id);

		}

#pragma endregion

#pragma region Transform

		struct Float3Interop {
			float x, y, z;
		};
		extern "C" void transform_get_position(uint32_t entityId, Float3Interop* outPos) {
			if (!ScriptRuntime::GetRegistry()->Has<ECS::TransformComponent>(entityId)) return;

			float3 position = ScriptRuntime::GetRegistry()->GetComponent<ECS::TransformComponent>(entityId).position;

			outPos->x = position.x;
			outPos->y = position.y;
			outPos->z = position.z;
		}

		extern "C" void transform_set_position(uint32_t entityId, Float3Interop* position) {
			if (!ScriptRuntime::GetRegistry()->Has<ECS::TransformComponent>(entityId)) return;

			ScriptRuntime::GetRegistry()->GetComponent<ECS::TransformComponent>(entityId).position = float3(position->x, position->y, position->z);
		}

#pragma endregion

#pragma region Time

		extern "C" float get_delta_time() {
			return GameTime::deltaTime;
		}
		extern "C" float get_time() {
			return GameTime::time;
		}

#pragma endregion

#pragma region Input

		extern "C" bool is_key_down(int key) {
			return false;
		}

		extern "C" bool is_key(int key) {
			return ScriptRuntime::GetInput()->GetKey((Input::KeyCode)key);
		}

		extern "C" bool is_key_up(int key) {
			return false;
		}

#pragma endregion

#pragma region Utility

		void output_to_console(MonoString* msg) {
			OutputDebugStringA(("[C# LOG]: " + std::string(mono_string_to_utf8(msg)) + "\n").c_str());
		}

#pragma endregion

		void RegisterCall(std::string name, const void* method) {
			mono_add_internal_call(name.c_str(), method);
		}
		void ScriptRuntime::RegisterInternalCalls() {
			// Utility
			RegisterCall("Engine.InternalCalls::output_to_console", output_to_console);

			// Transform
			//RegisterCall("Engine.InternalCalls::test_method", test_method);
			RegisterCall("Engine.InternalCalls::transform_set_position", transform_set_position);
			RegisterCall("Engine.InternalCalls::transform_get_position", transform_get_position);

			// Input
			RegisterCall("Engine.InternalCalls::is_key_down", is_key_down);
			RegisterCall("Engine.InternalCalls::is_key", is_key);
			RegisterCall("Engine.InternalCalls::is_key_up", is_key_up);

			// Entity
			RegisterCall("Engine.InternalCalls::entity_get_name", GetEntityName);

			// Time
			RegisterCall("Engine.InternalCalls::get_delta_time", get_delta_time);
		}
	}
}
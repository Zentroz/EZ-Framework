#include"ScriptObject.h"

namespace ENGINE {
	namespace SCRIPT {

		ScriptObject::ScriptObject(MonoObject* instance, MonoClass* klass) : m_instance(instance), m_class(klass) {
			onCreateMethod = mono_class_get_method_from_name(m_class, "OnCreate", 0);
			onUpdateMethod = mono_class_get_method_from_name(m_class, "OnUpdate", 0);
			onCollisionMethod = mono_class_get_method_from_name(m_class, "OnCollision", 0);
			onDestroyMethod = mono_class_get_method_from_name(m_class, "OnDestroy", 0);
		}

		void ScriptObject::InvokeCreate() { InvokeMethod(onCreateMethod, nullptr); }
		void ScriptObject::InvokeUpdate() { InvokeMethod(onUpdateMethod, nullptr); }
		void ScriptObject::InvokeCollision() { InvokeMethod(onCollisionMethod, nullptr); }
		void ScriptObject::InvokeDestroy() { InvokeMethod(onDestroyMethod, nullptr); }

		void ScriptObject::SetEntity(MonoObject* instance) {
			MonoProperty* prop = mono_class_get_property_from_name(m_class, "entity");
			MonoMethod* setter = mono_property_get_set_method(prop);

			void* args[1] = { instance };

			mono_runtime_invoke(setter, m_instance, args, nullptr);
		}

		void ScriptObject::InvokeMethod(MonoMethod* method, void* params) {
			if (!method) return;

			MonoObject* exception = nullptr;

			mono_runtime_invoke(method, m_instance, &params, &exception);

			if (exception != nullptr) EXCEPTION("Failed to invoke function.");
		}

	}
}
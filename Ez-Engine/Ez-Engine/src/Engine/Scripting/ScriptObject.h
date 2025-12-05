#ifndef SCRIPT_OBJECT_CLASS 
#define SCRIPT_OBJECT_CLASS

#include<mono/jit/jit.h>
#include<mono/metadata/assembly.h>

#include"Engine/Core/ErrorHandler.h"

namespace ENGINE {
	namespace SCRIPT {

		class ScriptObject {
		public:
			ScriptObject(MonoObject* instance, MonoClass* klass);

			void InvokeCreate();
			void InvokeUpdate();
			void InvokeCollision();
			void InvokeDestroy();

			void SetEntity(MonoObject* instance);

			ScriptObject(const ScriptObject&) = delete;
			ScriptObject& operator=(const ScriptObject&) = delete;
			ScriptObject(ScriptObject&&) = delete;
			ScriptObject& operator=(ScriptObject&&) = delete;


		private:
			MonoObject* m_instance;
			MonoClass* m_class;

			MonoMethod* onCreateMethod;
			MonoMethod* onUpdateMethod;
			MonoMethod* onCollisionMethod;
			MonoMethod* onDestroyMethod;

			void InvokeMethod(MonoMethod* method, void* params);
		};

	}
}

#endif

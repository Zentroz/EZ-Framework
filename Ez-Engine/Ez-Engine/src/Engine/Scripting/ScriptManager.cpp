#include"ScriptManager.h"
#include"ScriptObject.h"

namespace ENGINE {
	namespace SCRIPT {

        ECS::Registry* ScriptRuntime::registry = nullptr;
        Input* ScriptRuntime::input = nullptr;
        MonoDomain* ScriptRuntime::s_AppDomain = nullptr;

		void ScriptRuntime::Init() {
            EventBus::Subscribe<ComponentAddedEvent<ECS::ScriptComponent>>(
                [this](const ComponentAddedEvent<ECS::ScriptComponent>& data) -> void {
                    std::string key = data.component.nameSpace + "." + data.component.className;
                    CreateBehviourInstance(data.entity, key);
                }
            );
            EventBus::Subscribe<OnEntityCreated>(
                [this](const OnEntityCreated& data) -> void {
                    OnEntityCreatedCallback(data.entityId);
                }
            );
            EventBus::Subscribe<OnEntityDestroyed>(
                [this](const OnEntityDestroyed& data) -> void {
                    OnEntityDestroyedCallback(data.entityId);
                }
            );

			InitMono();
		}
        
        char* ReadBytes(const std::string& filepath, uint32_t* outSize)
        {
            std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

            if (!stream)
            {
                // Failed to open the file
                return nullptr;
            }

            std::streampos end = stream.tellg();
            stream.seekg(0, std::ios::beg);
            uint32_t size = end - stream.tellg();

            if (size == 0)
            {
                // File is empty
                return nullptr;
            }

            char* buffer = new char[size];
            stream.read((char*)buffer, size);
            stream.close();

            *outSize = size;
            return buffer;
        }

        MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
        {
            uint32_t fileSize = 0;
            char* fileData = ReadBytes(assemblyPath, &fileSize);

            // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
            MonoImageOpenStatus status;
            MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

            if (status != MONO_IMAGE_OK)
            {
                const char* errorMessage = mono_image_strerror(status);
                // Log some error message using the errorMessage data
                return nullptr;
            }

            MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
            mono_image_close(image);

            // Don't forget to free the file data
            delete[] fileData;

            return assembly;
        }

        void my_log_handler(const char* domain, const char* level, const char* message, mono_bool fatal, void* user_data) {
            OutputDebugStringA(("[MONO LOG]: " + std::string(message) + "\n").c_str());
        }

		void ScriptRuntime::InitMono() {
			mono_set_assemblies_path("mono/lib");

            mono_trace_set_level_string("debug");
            mono_trace_set_log_handler(my_log_handler, nullptr);

			MonoDomain* rootDomain = mono_jit_init("EZJITRuntime");
			if (rootDomain == nullptr)
			{
				// Error
				return;
			}

			s_RootDomain = rootDomain;

			char appDomainName[13] = "EZ-AppDomain";
			s_AppDomain = mono_domain_create_appdomain(appDomainName, nullptr);
            mono_domain_set(s_AppDomain, true);

            s_EngineAssembly = LoadCSharpAssembly("Script_Core.dll");
            s_GameAssembly = LoadCSharpAssembly("GameAssembly.dll");

            RegisterInternalCalls();

            entityKlass = mono_class_from_name(mono_assembly_get_image(s_EngineAssembly), "Engine", "Entity");
            entityBehaviour = mono_class_from_name(mono_assembly_get_image(s_EngineAssembly), "Engine", "EntityBehaviour");

            LoadGameComponentScripts();
		}

        void ScriptRuntime::ShutdownMono() {
            //mono_domain_unload(s_AppDomain);
            //s_AppDomain = nullptr;
            mono_jit_cleanup(s_RootDomain);
            s_RootDomain = nullptr;
        }

        void ScriptRuntime::LoadGameComponentScripts() {
            MonoImage* image = mono_assembly_get_image(s_GameAssembly);

            const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
            int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

            for (int32_t i = 0; i < numTypes; i++)
            {
                uint32_t cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

                const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
                const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

                MonoClass* klass = mono_class_from_name(image, nameSpace, name);

                if (!klass || klass == entityBehaviour) continue;

                if (mono_class_is_subclass_of(klass, entityBehaviour, true)) {
                    std::string key = std::string(nameSpace) + "." + name;
                    behaviourKlasses[key] = klass;
                }
            }
        }

        void ScriptRuntime::CreateBehviourInstance(Entity entity, std::string key) {
            MonoClass* klass = behaviourKlasses[key];
            MonoObject* monoObj = mono_object_new(s_AppDomain, klass);
            mono_runtime_object_init(monoObj);

            m_scripts.push_back(std::make_unique<ScriptObject>(monoObj, klass));
            m_scripts.back()->SetEntity(entityInstancesCS[entity]);
            m_scripts.back()->InvokeCreate();
        }

        void ScriptRuntime::OnEntityCreatedCallback(Entity id) {
            MonoImage* image = mono_assembly_get_image(s_EngineAssembly);

            MonoObject* instance = mono_object_new(s_AppDomain, entityKlass);
            mono_runtime_object_init(instance);

            // Setting Entity ID
            MonoClassField* field = mono_class_get_field_from_name(entityKlass, "id");
            uint32_t param =  id;
            mono_field_set_value(instance, field, &param);

            // Setting transform
            MonoClass* klass = mono_class_from_name(image, "Engine", "Transform");
            MonoObject* transformInstance = mono_object_new(s_AppDomain, klass);
            mono_runtime_object_init(transformInstance);

            MonoProperty* prop = mono_class_get_property_from_name(entityKlass, "transform");
            MonoMethod* setter = mono_property_get_set_method(prop);
            void* args[1] = { transformInstance };
            mono_runtime_invoke(setter, instance, args, nullptr);

            // Setting Entity property of transform
            MonoProperty* entityProp = mono_class_get_property_from_name(klass, "entity");
            MonoMethod* entitySetter = mono_property_get_set_method(entityProp);
            void* entityArgs[1] = { instance };
            mono_runtime_invoke(entitySetter, instance, entityArgs, nullptr);

            entityInstancesCS[id] = instance;
        }
        void ScriptRuntime::OnEntityDestroyedCallback(Entity id) {

        }

        void ScriptRuntime::Update() {
            for (auto& script : m_scripts) {
                script->InvokeUpdate();
            }
        }
	}
}